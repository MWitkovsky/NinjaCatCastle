/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleTextGenerator.cpp

	See NinjaCatCastleTextGenerator.h for a class description.
*/

#include "ninja_cat_castle_VS\stdafx.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "ninja_cat_castle\NinjaCatCastleTextGenerator.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\text\GameText.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

static const wstring	W_MOUSE_COORDS_TEXT = L"Mouse: (";
static const int		W_TEXT_X = 200;
static const int		W_TEXT_Y = 10;

void NinjaCatCastleTextGenerator::appendMouseCoords(Game *game)
{
	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	//textToGenerate.append(W_MOUSE_COORDS_TEXT);
	wss << input->getMousePoint()->x;
	wss << L", ";
	wss << input->getMousePoint()->y;
	wss << L")";
	wss << L"\n";
	//textToGenerate.append(wss.str());
}

void NinjaCatCastleTextGenerator::appendLivesCounter(Game *game){
	wstringstream wss;
	wss << L"x ";
	wss << game->getGSM()->getSpriteManager()->getPlayer()->getLives();
	livesCounter.append(wss.str());
}

void NinjaCatCastleTextGenerator::appendShurikenCounter(Game *game){
	wstringstream wss;
	wss << L"x ";
	wss << game->getGSM()->getSpriteManager()->getPlayer()->getShuriken();
	shurikenCounter.append(wss.str());
}

void NinjaCatCastleTextGenerator::appendTreatsCounter(Game *game){
	wstringstream wss;
	wss << L"x ";
	wss << game->getGSM()->getSpriteManager()->getPlayer()->getTreats();
	treatsCounter.append(wss.str());
}

/*
	initText - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void NinjaCatCastleTextGenerator::initText(Game *game)
{
	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	//appendMouseCoords(game);
	appendLivesCounter(game);
	appendShurikenCounter(game);
	appendTreatsCounter(game);

	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&livesLabel, LIVES_LABEL_X, LIVES_LABEL_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&livesCounter, LIVES_COUNTER_X, LIVES_COUNTER_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&shurikenLabel, SHURIKEN_LABEL_X, SHURIKEN_LABEL_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&shurikenCounter, SHURIKEN_COUNTER_X, SHURIKEN_COUNTER_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&treatsLabel, TREATS_LABEL_X, TREATS_LABEL_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addText(&treatsCounter, TREATS_COUNTER_X, TREATS_COUNTER_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void NinjaCatCastleTextGenerator::updateText(Game *game)
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	livesCounter.clear();
	shurikenCounter.clear();
	treatsCounter.clear();
	appendLivesCounter(game);
	appendShurikenCounter(game);
	appendTreatsCounter(game);
}