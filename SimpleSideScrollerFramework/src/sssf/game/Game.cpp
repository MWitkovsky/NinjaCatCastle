/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.cpp

	See Game.h for a class description.
*/
#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"
#include "sssf\text\TextGenerator.h"
#include "sssf\timer\GameTimer.h"

/*
	Game  - Constructor, this method begins the 
	construction and loading of all major game objects. 
	This method only needs to be called once, when the 
	application is first started.

	NOTE: This constructor will not initialize the custom
	game objects: graphics, input, os, & timer.
	These must be constructed separately and fed to this
	object using the init method. Doing it this way allows for
	more platform independence.
*/
Game::Game()
{
	// NOTE THAT graphics, input, os, & timer
	// ARE CUSTOM GAME OBJECTS. DEPENDING ON WHAT TECHNOLOGY 
	// IS TO BE USED THESE OBJECT SHOULD BE CONSTRUCTED
	// AND THEN FED TO THIS Game USING THE init METHOD
	gsm = new GameStateManager();
	gui = new GameGUI();
	text = new GameText();

	//Creates the fmod sound system
	FMOD::System_Create(&fmodSystem);
	fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
}

/*
	~Game - Desctructor, it recovers memory allocated for
	the game objects. If Game is deleted, the application
	is closing, so everything should be cleaned up. Note that
	this 
*/
Game::~Game() 
{
	// THESE WERE CREATED BY Game, AND SO SHOULD BE
	// DELETED BY GAME
	delete gsm;
	delete gui;
	delete text;

	// THE GAME AND PLATFORM STUFF SHOULD BE DELETED
	// BY WHOEVER CREATED THEM
}

/*
	initPlatformPlugins - This method is to be used to feed this Game
	the constructed, technology-specific, objects. 
*/
void Game::initPlatformPlugins(		GameGraphics *initGraphics,
									GameInput *initInput,
									GameOS *initOS,									
									GameTimer *initTimer)
{
	// INITIALIZE ALL OF THE GAME'S CUSTOM OBJECTS
	graphics = initGraphics;
	input = initInput;
	os = initOS;
	timer = initTimer;
}

/*
	reloadAllDevices - Windows applications must cooperate
	with other running applications, so when someone hits
	ALT-TAB and switches from a full-screen game, it might
	lose ownership of the graphics card. This method can
	be called when a full-screen application retains ownership
	of all necessary resources such that all necessary
	data (like textures, sound, music, etc.) can be reloaded.
*/
void Game::reloadAllDevices()
{
	graphics->reloadGraphics();

	// WE MIGHT ADD MORE LATER
}
	
/*
	runGameLoop - This is the game loop management method.
	It runs continuously while the game is active. Once per
	frame it instructs the major game objects to get
	user input, record user input, update the GUI state, 
	update the sprites' states using AI and input, perform
	collision detection and resolution (physics), render
	the screen, etc.

	This loop is timed such that everything is kept to a 
	consistent framerate, thus the game should run 
	consistently on all machines.
*/
void Game::runGameLoop()
{
	// FIRST PROFILE?
	bool firstTimeThroughLoop = true;

	// LET'S START THE TIMER FROM SCRATCH
	timer->resetTimer();

	// KEEP RENDERING UNTIL SOMEONE PULLS THE PLUG
	while(gsm->isAppActive())
	{
		// UPDATES THE FMOD SOUND SYSTEM
		fmodSystem->update();

		// MOVE ALONG WINDOWS MESSAGES, THIS ALLOWS
		// US TO GET USER INPUT
		os->processOSMessages();

		// GET USER INPUT AND UPDATE GAME, GUI, OR PLAYER
		// STATE OR WHATEVER IS NECESSARY
		input->processInput(this);

		// IT IS POSSIBLE THE USER REQUESTED A SHUTDOWN
		// OF THE APP, SO WE NEED TO CHECK AGAIN
		if (gsm->isAppActive())
		{
			// USE THE INPUT TO UPDATE THE GAME
			processGameData();

			// UPDATE MUSIC HANDLING
			processMusicLogic();

			// AND RENDER THE GAME
			graphics->renderGame(this);
		}
	}
}

/*
	processGameData - This method directs game logic to be
	executed or not, depending on the game state, it addition
	it triggers the building or render lists and game loop
	timing each frame.
*/
void Game::processGameData()
{
	// WE ONLY PERFORM GAME LOGIC IF THE GAME
	// IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		gsm->update(this);
	}
	else if (gsm->isGameLevelLoading())
	{
		gsm->goToGame();
	}

	// UPDATE TEXT
	TextGenerator *tg = text->getTextGenerator();
	tg->updateText(this);

	// BUILD THE RENDER LISTS
	graphics->fillRenderLists(this);

	// KEEP THE FRAME RATE CONSISTENT
	timer->timeGameLoop();
}

/*
	quitGame - When called, this method will force the unloading of all game data
	loaded for the current level and then return the user to the main menu.
*/
void Game::quitGame()
{
	// CLEAN UP ALL THE WORLD TEXTURES
	graphics->clearWorldTextures();
	gsm->getSpriteManager()->unloadSprites(this);
	gsm->getWorld()->unloadWorld();

	// WE'RE GOING BACK TO THE MAIN MENU
	gsm->goToMainMenu();
}

/*
	shutdown - This method starts the termination of the game application, 
	cascading shutdown proceedings through all the necessary classes, resulting
	in the release of all game resources.
*/
void Game::shutdown()
{
	// MAKE SURE THE GAME LOOP ENDS AND THAT THE GSM CLEANS
	// UP ALL THE MEMORY IT IS USING
	gsm->shutdown(this);

	// CLEAR OUT AND RELEASE THE GPU
	graphics->shutdown();

	// RELEASE GAME CONTROLLERS IF NECESSARY
	input->shutdown();

	// DESTROYS FMOD
	fmodSystem->release();

	// WE MAY SHUTDOWN OTHER THINGS HERE LIKE SOUND & MUSIC
	// RESOURCES IN THE FUTURE
	// AND KILL THE WINDOW
	os->shutdown();
}

/*
	startGame - When called, this method will load the first level of the
	game and start play. Note that one must make sure the currentLeve
*/
void Game::startGame()
{
	// AND UPDATE THE GAME STATE OF COURSE
	gsm->goToLoadLevel();	// NOTE THAT CURRENTLY THERE IS NO LEVEL FILE,
	
	// THAT'S ONE THING YOU'LL BE DOING
	if (currentLevelFileName == L"SideScrollerMetalLevel.tmx"){
		dataLoader->loadWorld(this, L"data/levels/SideScrollerMetal/", L"SideScrollerMetalLevel.tmx");
	}
	else if (currentLevelFileName == L"SideScrollerDesertLevel.tmx"){
		dataLoader->loadWorld(this, L"data/levels/SideScrollerDesert/", L"SideScrollerDesertLevel.tmx");
	}
}

//ALL LOGIC REGARDING MUSIC QUEUEING GOES HERE
//This will 99.9%-100% of the time be used to define what song
//to play after an intro to a song ends
void Game::processMusicLogic(){
	GameState gs = gsm->getCurrentGameState();
	if (gs == GS_MAIN_MENU || gs == GS_HELP_SCREEN || gs == GS_ABOUT_SCREEN){
		bool isMusicPlaying = false;
		musicChannel->isPlaying(&isMusicPlaying);
		if (!isMusicPlaying){
			musicChannel = playSong(MAIN_MENU_SONG, musicChannel);
		}
	}
	else if (gs == GS_GAME_IN_PROGRESS){
		bool isMusicPlaying = false;
		musicChannel->isPlaying(&isMusicPlaying);
		if (!isMusicPlaying){
			//Lets just keep these level file name checks hard coded...
			if (currentLevelFileName == L"SideScrollerMetalLevel.tmx"){
				musicChannel = playSong(LEVEL_1_SONG, musicChannel);
			}
		}
	}
}

//Plays a song in the specified channel (should always be the musicChannel)
FMOD::Channel* Game::playSong(const char* song, FMOD::Channel* songChannel){
	songChannel->stop(); //stops song currently playing in the music channel

	FMOD::Sound* newSong = 0;
	FMOD::Channel* newChannel = 0;

	fmodSystem->createStream(song, FMOD_DEFAULT, 0, &newSong);
	newSong->setMode(FMOD_LOOP_NORMAL); //loops

	fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel
	return newChannel; //returns newChannel
}

//Plays a song's intro in the music channel. This sets loop to off so we can just loop the main
//song over and over again without worry about the intro (the intro and the main song have to be
//separate sound files for this to work)
//Logic for playing the main song after the intro is handled by the method processMusicLogic() right above.
FMOD::Channel* Game::playSongIntro(const char* song, FMOD::Channel* songChannel){
	songChannel->stop(); //stops song currently playing in the music channel

	FMOD::Sound* newSong = 0;
	FMOD::Channel* newChannel = 0;

	fmodSystem->createSound(song, FMOD_DEFAULT, 0, &newSong);
	newSong->setMode(FMOD_LOOP_OFF); //doesn't loop

	fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel
	return newChannel; //returns newChannel
}

//Plays a sound in first available channel
//Doesn't return the sound channel because we actually don't care about managing it.
//The channel gets freed automatically as soon as the sound ends.
void Game::playSound(const char* sound){
	FMOD::Sound* newSound = 0;
	FMOD::Channel* newChannel = 0;

	fmodSystem->createSound(sound, FMOD_DEFAULT, 0, &newSound);
	newChannel->setMode(FMOD_LOOP_OFF); //doesn't loop, will automatically be destoryed when sound ends

	fmodSystem->playSound(newSound, 0, false, &newChannel);
}