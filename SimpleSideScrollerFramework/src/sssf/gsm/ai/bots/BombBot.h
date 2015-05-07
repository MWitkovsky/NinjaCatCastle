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
	b2Vec2	bombVelocity = b2Vec2(7.0f, 6.0f);
	b2Vec2	throwOffset = b2Vec2(0.5f, 0.5f);
	b2Vec2	seekRange = b2Vec2(8.0f, 6.0f);

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	BombBot(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	BombBot();
	~BombBot();
	void	throwBomb(Game *game);
	void	think(Game *game);

	// INLINED METHODS
	void	resetThinkCycles(){
		cyclesRemainingBeforeThinking = 200;
	}
};