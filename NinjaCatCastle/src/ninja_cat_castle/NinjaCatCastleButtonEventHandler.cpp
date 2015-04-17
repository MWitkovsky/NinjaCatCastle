/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleButtonEventHandler.cpp

	See NinjaCatCastleButtonEventHandler.h for a class description.
*/

#include "ninja_cat_castle_VS\stdafx.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "ninja_cat_castle\NinjaCatCastleButtonEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "fmod.hpp"

void NinjaCatCastleButtonEventHandler::handleButtonEvents(Game *game,
													wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		if (gsm->getCurrentGameState() == GS_SPLASH_SCREEN){
			game->setMusicChannel(game->playSongIntro(MAIN_MENU_SONG_INTRO, game->getMusicChannel()));
		}
		gsm->goToMainMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		//This shoulnd't actually be here because the song should start playing AFTER the level is loaded,
		//but you can just shift this next call to the place where the level is done loading so the music
		//plays when the player first sees the field. This is just an example of how the logic works :D
		game->setMusicChannel(game->playSongIntro(LEVEL_1_SONG_INTRO, game->getMusicChannel()));
		game->startGame();
	}

	else if (command.compare(W_HELP_COMMAND) == 0){
		game->getGSM()->goToHelpScreen();
	}

	else if (command.compare(W_ABOUT_COMMAND) == 0){
		game->getGSM()->goToAboutScreen();
	}

	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->getGSM()->getSpriteManager()->getPlayer()->setLives(3);
		game->setMusicChannel(game->playSongIntro(MAIN_MENU_SONG_INTRO, game->getMusicChannel()));
		game->quitGame();
	}
}