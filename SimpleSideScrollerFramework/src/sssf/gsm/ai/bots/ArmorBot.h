#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"
#include "Box2D\Box2D.h"

class ArmorBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking = 10;
	float32 walkVelocity = 1.5f;
	b2Vec2 attackRange = b2Vec2(2.0f, 2.0f);
	b2Vec2 approachRange = b2Vec2(12.0f, 2.0f);
	b2Fixture* attackBox;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	ArmorBot(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	ArmorBot();
	~ArmorBot();
	boolean	walk(Game *game, float32 diffX, float32 diffY);
	void	think(Game *game);

	// INLINED METHODS
	void	resetThinkCycles(){
		cyclesRemainingBeforeThinking = 200;
	}

	void resetThinkCyclesBlock(){
		cyclesRemainingBeforeThinking = 100;
	}

	void setAttackBox(b2Fixture* attackBox){
		this->attackBox = attackBox;
	}

	boolean isBotAttacking(){
		return (currentState == L"ATTACK_LEFT_A" || currentState == L"ATTACK_RIGHT_A"
			|| currentState == L"WINDUP_LEFT" || currentState == L"WINDUP_RIGHT"
			|| currentState == L"ATTACK_RECOVER_LEFT" || currentState == L"ATTACK_RECOVER_RIGHT");
	}

	boolean isBlocking(){
		return (currentState == L"BLOCK_LEFT" || currentState == L"BLOCK_RIGHT");
	}

	boolean isVulnerable(){
		return (currentState == L"ATTACK_RECOVER_LEFT" || currentState == L"ATTACK_RECOVER_RIGHT");
	}

	b2Fixture* getAttackBox(){
		return attackBox;
	}
};