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

void PropellerBot::chooseFlyTrajectory(){

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
		if (cyclesRemainingBeforeThinking == 0)
		{
			GameStateManager *gsm = game->getGSM();
			b2Vec2 playerPos = gsm->getSpriteManager()->getPlayer()->getBody()->GetPosition();
		}
		else{
			cyclesRemainingBeforeThinking--;
		}
	}
}