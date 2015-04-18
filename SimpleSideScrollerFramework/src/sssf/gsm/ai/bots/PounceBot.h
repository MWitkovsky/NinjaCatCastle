#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"
#include "Box2D\Box2D.h"

class PounceBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking = 200;
	unsigned int maxVelocity; 
	unsigned int animationRandomizer;
	float32 jumpVelocityX = 8.0f;
	float32 jumpVelocityY = 7.0f;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	PounceBot(unsigned int initMin,
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	PounceBot();
	~PounceBot();
	Bot*	clone();
	void	resetThinkCycles();
	void	Jump(boolean jumpRight);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
};