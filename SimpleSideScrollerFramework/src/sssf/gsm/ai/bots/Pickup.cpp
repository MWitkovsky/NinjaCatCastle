#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\Pickup.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
This is the public constructor used by other classes for
creating these types of bots.
*/
Pickup::Pickup(boolean health)
{
	this->health = health;
}

void Pickup::calculateBob(){
	b2Vec2 currentPosition = body->GetPosition();
	float32 currentVX = 0.0f;
	float32 currentVY = body->GetLinearVelocity().y;
	//init the movement first time
	if (currentVY == 0.0f){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}

	else if (currentPosition.y <= (originalPosition.y - bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, bobVelocity));
	}
	else if (currentPosition.y >= (originalPosition.y + bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}
	else{
		body->SetLinearVelocity(b2Vec2(currentVX, currentVY));
	}
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void Pickup::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	calculateBob();
	animationCounter++;
}