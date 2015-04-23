/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "ninja_cat_castle_VS\stdafx.h"
#include "sssf\text\TextGenerator.h"

class NinjaCatCastleTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring livesLabel = L"LIVES";
	wstring livesCounter;
	wstring treatsLabel = L"TREATS";
	wstring treatsCounter;

public:
	// NOTHING TO INITIALIZE OR DESTROY
	NinjaCatCastleTextGenerator()	{}
	~NinjaCatCastleTextGenerator()	{}

	// DEFINED IN NinjaCatCastleTextGenerator.cpp
	void appendMouseCoords(Game *game);
	void appendLivesCounter(Game *game);
	void appendTreatsCounter(Game *game);
	void initText(Game *game);
	void updateText(Game *game);
};