#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class Pickup : public Bot
{
private:
	b2Vec2 originalPosition = b2Vec2(0.0f, 0.0f);
	float32 bobDistance = 0.1f;
	float32 bobVelocity = 0.375f;
	boolean health = false;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	Pickup(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	Pickup(boolean health);
	~Pickup();
	void	calculateBob();
	void	think(Game *game);

	// INLINED METHODS
	boolean isHealth(){
		return health;
	}

	void setOriginalPosition(b2Vec2 originalPosition){
		this->originalPosition = originalPosition;
	}
};