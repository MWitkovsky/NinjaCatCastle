/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleButtonEventHandler.h

	This is a custom button event handler. Each game can choose
	to use different buttons and respond differently to when
	buttons are pressed. This class will handle button presses for
	the NinjaCatCastleGame following the ButtonEventHandler pattern.
*/

#pragma once
#include "ninja_cat_castle_VS\stdafx.h"
#include "sssf\input\ButtonEventHandler.h"

class Game;

class NinjaCatCastleButtonEventHandler: public ButtonEventHandler
{
public:
	// AS-IS, THIS CLASS HAS NO DATA, SO THERE IS NOTHING
	// TO INITIALIZE OR DESTROY
	NinjaCatCastleButtonEventHandler()	{}
	~NinjaCatCastleButtonEventHandler()	{}

	// DEFINED INSIDE NinjaCatCastleButtonEventHandler.cpp
	void handleButtonEvents(Game *game, 
							wstring command);
};