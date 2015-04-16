/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "ninja_cat_castle_VS\stdafx.h"

// NinjaCatCastle GAME INCLUDES
#include "ninja_cat_castle\NinjaCatCastleButtonEventHandler.h"
#include "ninja_cat_castle\NinjaCatCastleCollisionListener.h"
#include "ninja_cat_castle\NinjaCatCastleDataLoader.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "ninja_cat_castle\NinjaCatCastleKeyEventHandler.h"
#include "ninja_cat_castle\NinjaCatCastleTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
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

#include "fmod.hpp"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *ninjaCatCastleGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	NinjaCatCastleDataLoader *ninjaCatCastleDataLoader = new NinjaCatCastleDataLoader();
	ninjaCatCastleDataLoader->initWinHandle(hInstance, nCmdShow);
	ninjaCatCastleGame->setDataLoader(ninjaCatCastleDataLoader);
	ninjaCatCastleDataLoader->loadGame(ninjaCatCastleGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	ninjaCatCastleDataLoader->loadGUI(ninjaCatCastleGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	NinjaCatCastleButtonEventHandler *ninjaCatCastleButtonHandler = new NinjaCatCastleButtonEventHandler();
	GameGUI *gui = ninjaCatCastleGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)ninjaCatCastleButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	NinjaCatCastleKeyEventHandler *ninjaCatCastleKeyHandler = new NinjaCatCastleKeyEventHandler();
	ninjaCatCastleGame->getInput()->registerKeyHandler((KeyEventHandler*)ninjaCatCastleKeyHandler);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS
	NinjaCatCastleCollisionListener *ninjaCatCastleCollisionListener = new NinjaCatCastleCollisionListener();
	ninjaCatCastleGame->getGSM()->getPhysics()->getWorld()->SetContactListener(ninjaCatCastleCollisionListener);
	ninjaCatCastleGame->getGSM()->getPhysics()->getWorld()->SetContactFilter(ninjaCatCastleCollisionListener);

	FMOD::Sound      *sound1 = NULL; //sound that will be loaded and played
	FMOD::Channel    *channel = 0;

	FMOD::System* wow = ninjaCatCastleGame->getSoundSystem();
	wow->createSound("data/song 3.mp3", FMOD_DEFAULT, 0, &sound1);
	sound1->setMode(FMOD_LOOP_NORMAL);

	wow->playSound(sound1, 0, false, &channel);

	// START THE GAME LOOP
	ninjaCatCastleGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)ninjaCatCastleGame->getOS();
	delete (WindowsInput*)ninjaCatCastleGame->getInput();
	delete (WindowsTimer*)ninjaCatCastleGame->getTimer();
	delete (DirectXGraphics*)ninjaCatCastleGame->getGraphics();
	delete (NinjaCatCastleTextGenerator*)ninjaCatCastleGame->getText()->getTextGenerator();
	delete ninjaCatCastleButtonHandler;
	delete ninjaCatCastleKeyHandler;
	delete ninjaCatCastleGame;

	// AND RETURN
	return 0;
}