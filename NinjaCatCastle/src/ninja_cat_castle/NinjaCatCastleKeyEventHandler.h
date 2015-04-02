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

class Game;

class NinjaCatCastleKeyEventHandler : public KeyEventHandler
{
public:
	NinjaCatCastleKeyEventHandler()		{}
	~NinjaCatCastleKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
};