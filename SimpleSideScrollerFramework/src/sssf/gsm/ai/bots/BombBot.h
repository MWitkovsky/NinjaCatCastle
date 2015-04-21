#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"
#include "Box2D\Box2D.h"

class BombBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking = 10;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	BombBot(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	BombBot();
	~BombBot();
	void	think(Game *game);

	// INLINED METHODS
	void	resetThinkCycles(){
		cyclesRemainingBeforeThinking = 200;
	}
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
};