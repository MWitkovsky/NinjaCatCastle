/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "ninja_cat_castle_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"
#include "Box2D\Box2D.h"

class Game;

class NinjaCatCastleKeyEventHandler : public KeyEventHandler
{
private:
	int left = 0;
	int upleft = 1;
	int up = 2;
	int upright = 3;
	int right = 4;
	b2Vec2	projectileVelocity = b2Vec2(15.0f, 15.0f);

public:
	NinjaCatCastleKeyEventHandler()		{}
	~NinjaCatCastleKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
	void handleShurikenThrow(Game *game, int throwCode);
};