#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class PropellerBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking = 20;
	unsigned int changeDirectionWait = 10;
	b2Vec2 originalPosition = b2Vec2(0.0f, 0.0f);
	float32 maxDistance = 3.0f;
	float32 bobDistance = 0.2f;
	float32 flyVelocity = 1.5f;
	float32 bobVelocity = 0.75f;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	PropellerBot(unsigned int initMin,
						unsigned int initMax, 
						unsigned int initMaxVelocity);

public:
	PropellerBot();
	~PropellerBot();
	void	shoot(Game *game);
	void	calculateBob();
	void	chooseFlyTrajectory();
	void	think(Game *game);

	// INLINED METHODS
	void setOriginalPosition(b2Vec2 originalPosition){
		this->originalPosition = originalPosition;
	}
	void resetThinkCycles(){
		cyclesRemainingBeforeThinking = 200;
	}
	void resetChangeDirectionWait(){
		changeDirectionWait = 120;
	}
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
};