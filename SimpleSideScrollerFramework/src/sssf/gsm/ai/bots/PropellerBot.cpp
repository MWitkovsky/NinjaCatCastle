#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\PropellerBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
PropellerBot::PropellerBot()
{
}

void PropellerBot::shoot(Game *game){

}

void PropellerBot::calculateBob(){
	b2Vec2 currentPosition = body->GetPosition();
	float32 currentVX = body->GetLinearVelocity().x;
	float32 currentVY = body->GetLinearVelocity().y;
	//init the movement first time
	if (currentVY == 0.0f){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}

	else if (currentPosition.y <= (originalPosition.y - bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, bobVelocity));
	}
	else if(currentPosition.y >= (originalPosition.y + bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}
	else{
		body->SetLinearVelocity(b2Vec2(currentVX, currentVY));
	}
}

void PropellerBot::chooseFlyTrajectory(){
	b2Vec2 currentPosition = body->GetPosition();
	float32 currentVY = body->GetLinearVelocity().y;
	if (currentPosition.x <= (originalPosition.x - maxDistance)){
		if (changeDirectionWait){
			body->SetLinearVelocity(b2Vec2(0.0f, currentVY));
			changeDirectionWait--;
		}
		else{
			body->SetLinearVelocity(b2Vec2(flyVelocity, currentVY));
			resetChangeDirectionWait();
		}
	}
	else if (currentPosition.x >= originalPosition.x){
		if (changeDirectionWait){
			body->SetLinearVelocity(b2Vec2(0.0f, currentVY));
			changeDirectionWait--;
		}
		else{
			body->SetLinearVelocity(b2Vec2(-flyVelocity, currentVY));
			resetChangeDirectionWait();
		}
	}
	else{
		if (body->GetLinearVelocity().x <= 0.0f){
			body->SetLinearVelocity(b2Vec2(-flyVelocity, currentVY));
		}
		else{
			body->SetLinearVelocity(b2Vec2(flyVelocity, currentVY));
		}
		
	}
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void PropellerBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	if (!isDead()){
		if (!isAttacking()){
			chooseFlyTrajectory();
		}
		calculateBob();
		if (cyclesRemainingBeforeThinking == 0)
		{
			GameStateManager *gsm = game->getGSM();
			b2Vec2 playerPos = gsm->getSpriteManager()->getPlayer()->getBody()->GetPosition();
		}
		else{
			cyclesRemainingBeforeThinking--;
		}
	}
	animationCounter++;
}