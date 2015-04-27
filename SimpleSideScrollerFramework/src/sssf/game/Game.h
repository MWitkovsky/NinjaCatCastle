/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.h

	The class is the central object of the game
	application. It plays two important roles:

	1) It manages all major game objects (Game...), which
	essentially manage the game data and communicates with
	external technologies like graphics cards and input
	devices. The objects can be accessed via inlined public
	methods, i.e. getGraphics(), getInput(), getTimer(), etc.

	2) It runs the game loop, calling the appropriate
	methods in the other major objects as needed. The
	runGameLoop method iterates once per frame, invoking
	all necessary methods needed each frame to run the game.

	NOTE: I have deliberately avoided making this class 
	technology specific. I would like to be able to change
	the way the graphics are rendered (i.e. DirectX or OpenGL)
	if I want to without having to change this or most other
	classes.
*/

#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\state\GameState.h"
#include "fmod.hpp"
#include "luaplus.h"
using namespace LuaPlus;

// FORWARD DECLARATIONS OF CLASSES USED BY GAME
class GameDataLoader;
class GameGraphics;
class GameInput;
class GameOS;
class GameTimer;
class GameStateManager;
class GameGUI;
class GameText;

static const float PI = 3.1415926535897932384626433f;

//File that all the following values are loaded from
static const char*		W_LUA_FILE = "data/NinjaCatCastle.lua";

//FMOD and sound stuff
extern char*	SPLASH_JINGLE;
extern char*	MAIN_MENU_SONG_INTRO;
extern char*	MAIN_MENU_SONG;
extern char*	LEVEL_1_SONG_INTRO;
extern char*	LEVEL_1_SONG;
extern char*	LEVEL_2_SONG_INTRO;
extern char*	LEVEL_2_SONG;
extern char*	LEVEL_3_SONG_INTRO;
extern char*	LEVEL_3_SONG;

extern char*	SOUND_ARROWSHOT;
extern char*	SOUND_BOMB_EXPLOSION1;
extern char*	SOUND_BOMB_EXPLOSION2;
extern char*	SOUND_CAT_POUNCE;
extern char*	SOUND_PROPELLER;
extern char*	SOUND_HIT;
extern char*	SOUND_HIT2;
extern char*	SOUND_MC_DEATH;
extern char*	SOUND_MC_JUMP;
extern char*	SOUND_SLASH1;
extern char*	SOUND_SLASH2;
extern char*	SOUND_BIGSLASH;

// HERE IS THE NinjaCatCastle GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
extern wstring	W_LEVEL_1_DIR;
extern wstring	W_LEVEL_1_NAME;
extern wstring	W_LEVEL_2_DIR;
extern wstring	W_LEVEL_2_NAME;
extern wstring	W_LEVEL_3_DIR;
extern wstring	W_LEVEL_3_NAME;
extern wstring	W_INIT_FILE;
extern wstring	W_GUI_INIT_FILE;
extern wstring	W_LEVEL_1_PATH;
extern string	W_DEBUG_FILE;

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
extern wstring	W_TITLE;
extern wstring	W_SCREEN_WIDTH;
extern wstring	W_SCREEN_HEIGHT;
extern wstring	W_USE_FULLSCREEN_MODE;
extern wstring	W_TEXT_FONT_SIZE;
extern wstring	W_VIEWPORT_OFFSET_X;
extern wstring	W_VIEWPORT_OFFSET_Y;
extern wstring	W_TOGGLE_OFFSET_Y;
extern wstring	W_FONT_COLOR_RED;
extern wstring	W_FONT_COLOR_GREEN;
extern wstring	W_FONT_COLOR_BLUE;
extern wstring	W_COLOR_KEY_RED;
extern wstring	W_COLOR_KEY_GREEN;
extern wstring	W_COLOR_KEY_BLUE;

// CONSTANTS FOR ARTWORK
extern wstring	W_GO_TO_MM_COMMAND;
extern wstring	W_START_COMMAND;
extern wstring	W_HELP_COMMAND;
extern wstring  W_ABOUT_COMMAND;
extern wstring	W_EXIT_COMMAND;
extern wstring	W_QUIT_COMMAND;
extern wstring	W_CURSOR_PATH;
extern wstring	W_SPLASH_SCREEN_PATH;
extern wstring	W_MAIN_MENU_PATH;
extern wstring	W_HELP_SCREEN_PATH;
extern wstring	W_ABOUT_SCREEN_PATH;
extern wstring	W_HELP_IMAGE_PATH;
extern wstring	W_HELP_IMAGE_MO_PATH;
extern wstring	W_ABOUT_IMAGE_PATH;
extern wstring	W_ABOUT_IMAGE_MO_PATH;
extern wstring	W_START_IMAGE_PATH;
extern wstring	W_START_IMAGE_MO_PATH;
extern wstring	W_EXIT_IMAGE_PATH;
extern wstring	W_EXIT_IMAGE_MO_PATH;
extern wstring	W_QUIT_IMAGE_PATH;
extern wstring	W_QUIT_IMAGE_MO_PATH;
extern wstring	W_LIVES_HEAD_PATH;
extern wstring	W_TREAT_ICON_PATH;

// FOR OUR SPRITE
// ...kind of neglected to bother listing them all here. oops.
// I like how the states stand out as a different color anyway. Easier to read.
extern wstring	SPRITE_TYPES_DIR;
extern wstring	SPRITE_TYPES_LIST;

// FOR THE GAME WORLD
extern float		MAX_VIEWPORT_AXIS_VELOCITY;
extern float		W_GRAVITY;
extern float		JUMP_SPEED;
extern float		PLAYER_SPEED;
extern float		PLAYER_MELEE_RANGE;
extern int			MIN_FPS;
extern int			MAX_FPS;
extern int			FPS_INC;

//Text coordinates
extern int			LIVES_LABEL_X;
extern int			LIVES_LABEL_Y;
extern int			LIVES_COUNTER_X;
extern int			LIVES_COUNTER_Y;
extern int			TREATS_LABEL_X;
extern int			TREATS_LABEL_Y;
extern int			TREATS_COUNTER_X;
extern int			TREATS_COUNTER_Y;

//TODO: put all other button positions below here..
extern int			START_BUTTON_X;
extern int			START_BUTTON_Y;
extern int			METER_TO_PIXEL_SCALE;
extern int			LIVES_HEAD_X;
extern int			LIVES_HEAD_Y;
extern int			TREAT_ICON_X;
extern int			TREAT_ICON_Y;

//FMod Stuff
extern FMOD::System*		fmodSystem; //handle to FMOD engine
extern FMOD::Channel*		musicChannel;
extern FMOD::Channel*		introChannel; //needed for seamless looping

// USED FOR MOVEMENT
// LEAVE THESE HERE
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int G_KEY = (unsigned int)'G';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int K_KEY = (unsigned int)'K';
const unsigned int J_KEY = (unsigned int)'J';
const unsigned int H_KEY = (unsigned int)'H';
const unsigned int ONE_KEY = (unsigned int)'1';
const unsigned int TWO_KEY = (unsigned int)'2';
const unsigned int THREE_KEY = (unsigned int)'3';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int CTRL_KEY = VK_CONTROL;

class Game
{
private:
	// THE GameStateManager DEALS WITH ALL THE
	// GAME-PLAY AND GAME-WORLD STUFF, LIKE AI,
	// PHYSICS, AND GAME WORLD CONSTRUCTION. IT
	// ALSO KNOWS THE CURRENT GAME STATE
	GameStateManager	*gsm;

	// MANAGES ALL THE GUIS
	GameGUI				*gui;

	// MANAGES TEXT FOR RENDERING AND FILE I/O
	GameText			*text;

	// THESE GAME OBJECTS ALL REQUIRE CUSTOM DEFINITIONS, EITHER
	// BECAUSE THEY DEAL WITH PLATFORM-SPECIFIC THINGS, OR GAME-
	// SPECIFIC THINGS
	GameDataLoader		*dataLoader;
	GameGraphics		*graphics;
	GameInput			*input;
	GameOS				*os;
	GameTimer			*timer;

	// THIS WILL KEEP TRACK OF THE NAME OF THE LEVEL FILE
	// THAT IS CURRENTLY BEING USED
	wstring				currentLevelFileName;

public:
	// INLINED ACCESSOR METHODS - THE REAL THINGS
	GameStateManager*	getGSM()				{ return gsm;								}
	GameGUI*			getGUI()				{ return gui;								}
	GameText*			getText()				{ return text;								}
	FMOD::System *		getSoundSystem()		{ return fmodSystem; }
	// INLINED ACCESSOR METHODS - ACTUALLY SUB-CLASSES
	GameDataLoader*		getDataLoader()			{ return dataLoader;						}
	GameGraphics*		getGraphics()			{ return graphics;							}
	GameInput*			getInput()				{ return input;								}
	GameOS*				getOS()					{ return os;								}
	GameTimer*			getTimer()				{ return timer;								}

	//FMOD METHODS
	FMOD::Channel* getMusicChannel()			{ return musicChannel; }
	void setMusicChannel(FMOD::Channel* newMusicChannel){
		musicChannel = newMusicChannel;
	}

	// AND FOR GETTING THE NAME OF THE FILE USED FOR LOADING THIS LEVEL
	wstring				getCurrentLevelFileName()	{ return currentLevelFileName;			}

	// INLINED MUTATOR METHODS
	void	setDataLoader(GameDataLoader *initDL) { dataLoader = initDL; }
	void	setCurrentLevelFileName(wstring initCurrentLevelFileName) { currentLevelFileName = initCurrentLevelFileName; }

	// METHODS DEFINED IN Game.cpp
	Game();
	~Game();
	void	initPlatformPlugins(	GameGraphics *initGraphics, 
									GameInput *initInput, 
									GameOS *initOS,
									GameTimer *initTimer);
	void	processGameData();
	void	quitApp();
	void	quitGame();
	void	reloadAllDevices();
	void	runGameLoop();
	void	shutdown();
	void	startGame();
	void	processMusicLogic();
	FMOD::Channel*	playSongIntro(const char* song, FMOD::Channel* songChannel);
	FMOD::Channel*	queueSong(const char* song, FMOD::Channel* songChannel);
	FMOD::Channel*	playSongNoIntro(const char* song, FMOD::Channel* songChannel);
	void	playSound(const char* sound);
	void	readLUA(const char* fileName);
};