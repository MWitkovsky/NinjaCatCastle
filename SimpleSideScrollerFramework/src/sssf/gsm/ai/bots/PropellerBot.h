#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class PropellerBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	b2Vec2 originalPosition = b2Vec2(0.0f, 0.0f);

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	PropellerBot(unsigned int initMin,
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	PropellerBot();
	~PropellerBot();
	void	shoot(Game *game);
	void	chooseFlyTrajectory();
	void	think(Game *game);

	// INLINED METHODS
	void setOriginalPosition(b2Vec2 originalPosition){
		this->originalPosition = originalPosition;
	}
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
};