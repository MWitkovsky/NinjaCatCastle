#include "ninja_cat_castle_VS\stdafx.h"

// BugginOut GAME INCLUDES
#include "ninja_cat_castle\NinjaCatCastleButtonEventHandler.h"
#include "ninja_cat_castle\NinjaCatCastleDataLoader.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "ninja_cat_castle\NinjaCatCastleKeyEventHandler.h"
#include "ninja_cat_castle\NinjaCatCastleTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\bots\RandomJumpingBot.h"
#include "sssf\gsm\ai\bots\RandomFloatingBot.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void NinjaCatCastleDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *ninjaCatCastleOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *ninjaCatCastleGraphics = new DirectXGraphics(game);
	ninjaCatCastleGraphics->init(screenWidth, screenHeight);
	ninjaCatCastleGraphics->initGraphics(ninjaCatCastleOS, useFullscreen);
	ninjaCatCastleGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(ninjaCatCastleGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *ninjaCatCastleInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *ninjaCatCastleTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins((GameGraphics*)ninjaCatCastleGraphics,
		(GameInput*)ninjaCatCastleInput,
		(GameOS*)ninjaCatCastleOS,
		(GameTimer*)ninjaCatCastleTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	NinjaCatCastleTextGenerator *ninjaCatCastleTextGenerator = new NinjaCatCastleTextGenerator();
	ninjaCatCastleTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)ninjaCatCastleTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void NinjaCatCastleDataLoader::initColors(GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void NinjaCatCastleDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void NinjaCatCastleDataLoader::loadWorld(Game *game, wstring dir, wstring name)
{
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, dir, name);

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - IT WOULD BE BETTER TO LOAD THIS STUFF FROM A FILE
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSprite *player = spriteManager->getPlayer();

	// NOTE THAT RED BOX MAN IS SPRITE ID 2
	if (!spriteManager->getPlayer()->getSpriteType()){
		physics->addCollidableObject(player);
		AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(2);
		player->setSpriteType(playerSpriteType);
		player->setAlpha(255);
		player->setCurrentState(IDLE);
		PhysicalProperties *playerProps = player->getPhysicalProperties();
		playerProps->setX(PLAYER_INIT_X);
		playerProps->setY(PLAYER_INIT_Y);
		playerProps->setOriginalX(PLAYER_INIT_X);
		playerProps->setOriginalY(PLAYER_INIT_Y);
		playerProps->setVelocity(0.0f, 0.0f);
		playerProps->setAccelerationX(0);
		playerProps->setAccelerationY(0);
		playerProps->setAffectedByGravity(true);
		player->setOnTileThisFrame(false);
		player->setOnTileLastFrame(false);
		player->affixTightAABBBoundingVolume();

		AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(1);
		// AND LET'S ADD A BUNCH OF RANDOM JUMPING BOTS, FIRST ALONG
		// A LINE NEAR THE TOP

		// UNCOMMENT THE FOLLOWING CODE BLOCK WHEN YOU ARE READY TO ADD SOME BOTS

		for (int i = 4; i <= 10; i++)
		{
			float botX = 400.0f + (i * 100.0f);
			float botY = 200.0f;
			makeRandomJumpingBot(game, botSpriteType, botX, botY);
		}
		botSpriteType = spriteManager->getSpriteType(0);
		for (int i = 16; i <= 22; i++){
			float botX = 400.0f + (i * 100.0f);
			float botY = 200.0f;
			makeRandomJumpingBot(game, botSpriteType, botX, botY);
		}

		makeRandomFloatingBot(game, spriteManager->getSpriteType(3), 1000, 500);
	}
	else{
		player->setAlpha(255);
		player->setCurrentState(IDLE);
		PhysicalProperties *playerProps = player->getPhysicalProperties();
		playerProps->setX(playerProps->getOriginalX());
		playerProps->setY(playerProps->getOriginalY());
		playerProps->setVelocity(0.0f, 0.0f);
		playerProps->setAccelerationX(0);
		playerProps->setAccelerationY(0);
		player->setOnTileThisFrame(false);
		player->setOnTileLastFrame(false);
		player->affixTightAABBBoundingVolume();
		if (!player->isControllable()){
			player->toggleControllable();
		}

		player->setHP(10);

		list<Bot*>::iterator botsIterator = spriteManager->getBotsIterator();
		while (botsIterator != spriteManager->getEndOfBotsIterator()){
			(*botsIterator)->getPhysicalProperties()->setPosition((*botsIterator)->getPhysicalProperties()->getOriginalX(), (*botsIterator)->getPhysicalProperties()->getOriginalY());
			(*botsIterator)->setCurrentState((*botsIterator)->getOriginalState());
			(*botsIterator)->affixTightAABBBoundingVolume();
			botsIterator++;
		}
	}

	// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
	/*makeRandomJumpingBot(game, botSpriteType, 400, 100);
	makeRandomJumpingBot(game, botSpriteType, 200, 400);
	makeRandomJumpingBot(game, botSpriteType, 400, 400);
	makeRandomJumpingBot(game, botSpriteType, 800, 700);
	makeRandomJumpingBot(game, botSpriteType, 900, 700);
	makeRandomJumpingBot(game, botSpriteType, 1000, 700);
	makeRandomJumpingBot(game, botSpriteType, 100, 1000);
	makeRandomJumpingBot(game, botSpriteType, 300, 1000);	
	makeRandomJumpingBot(game, botSpriteType, 500, 1000);
	makeRandomJumpingBot(game, botSpriteType, 100, 1400);
	makeRandomJumpingBot(game, botSpriteType, 400, 1400);	
	makeRandomJumpingBot(game, botSpriteType, 700, 1400);*/

	// AND THEN A BUNCH LINED UP NEAR THE LEVEL EXIT
	/*for (int i = 0; i < 14; i++)
		makeRandomJumpingBot(game, botSpriteType, 1700.0f + (i*100.0f), 1300.0f);
		*/
}

void NinjaCatCastleDataLoader::makeRandomJumpingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	RandomJumpingBot *bot = new RandomJumpingBot(physics, 30, 120, 40);
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	pp->setOriginalX(initX);
	pp->setOriginalY(initY);
	bot->setOriginalState(L"JUMPING");
	pp->setAffectedByGravity(true);
	bot->setSpriteType(randomJumpingBotType);
	bot->setCurrentState(JUMPING);
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();
}

void NinjaCatCastleDataLoader::makeRandomFloatingBot(Game *game, AnimatedSpriteType *randomJumpingBotType, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	RandomFloatingBot *bot = new RandomFloatingBot(physics, 30, 120, 5);
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	pp->setOriginalX(initX);
	pp->setOriginalY(initY);
	pp->setAffectedByGravity(false);
	bot->setSpriteType(randomJumpingBotType);
	bot->setCurrentState(L"ATTACKING_DOWN");
	bot->setOriginalState(L"ATTACKING_DOWN");
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();
}

/*
	initBugginOutGUI - This method builds a GUI for the BugginOut Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void NinjaCatCastleDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initHelpScreen(game, gui, guiTextureManager);
	initAboutScreen(game, gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void NinjaCatCastleDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE KUNAI CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void NinjaCatCastleDataLoader::initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void NinjaCatCastleDataLoader::initMainMenu(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD A START BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	int normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							START_BUTTON_X,
							START_BUTTON_Y,
							0,
							255,
							219,
							49,
							false,
							W_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A HELP BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_MO_PATH);

	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		START_BUTTON_X,
		START_BUTTON_Y + 100,
		0,
		255,
		219,
		49,
		false,
		W_HELP_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN ABOUT BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_MO_PATH);

	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		START_BUTTON_X + 300,
		START_BUTTON_Y + 100,
		0,
		255,
		219,
		49,
		false,
		W_ABOUT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		START_BUTTON_X,
		START_BUTTON_Y + 200,
		0,
		255,
		219,
		49,
		false,
		W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

/*
	initHelpScreen - initializes the game's help screen gui.
*/
void NinjaCatCastleDataLoader::initHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager){
	// NOW, FIRST LET'S ADD A HELP SCREEN GUI
	ScreenGUI *helpScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_HELP_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE RETURN BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_GO_TO_MM_COMMAND);
	helpScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_HELP_SCREEN, helpScreenGUI);
}

/*
initAboutScreen - initializes the game's help screen gui.
*/
void NinjaCatCastleDataLoader::initAboutScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager){
	// NOW, FIRST LET'S ADD A HELP SCREEN GUI
	ScreenGUI *aboutScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_ABOUT_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE RETURN BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_GO_TO_MM_COMMAND);
	aboutScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_ABOUT_SCREEN, aboutScreenGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void NinjaCatCastleDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							127,
							62,
							false,
							W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	//Add the lives counter head graphic
	OverlayImage *overlayImageToAdd = new OverlayImage();
	overlayImageToAdd->x = 900;
	overlayImageToAdd->y = 725;
	overlayImageToAdd->imageID = guiTextureManager->loadTexture(W_LIVES_HEAD_PATH);
	overlayImageToAdd->height = 32;
	overlayImageToAdd->width = 32;
	overlayImageToAdd->alpha = 255;
	overlayImageToAdd->z = 0;

	inGameGUI->addOverlayImage(overlayImageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void NinjaCatCastleDataLoader::initViewport(GameGUI *gui, map<wstring, wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}