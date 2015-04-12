#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

PounceBot::PounceBot()
{
	resetThinkCycles();
}

/*
	clone - this method makes another RandomJumpingBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* PounceBot::clone()
{
	PounceBot *botClone = new PounceBot();
	return botClone;
}

/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/
void PounceBot::resetThinkCycles()
{
	cyclesRemainingBeforeThinking = 200;
}

/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void PounceBot::Jump(Physics *physics)
{
	//pp.setVelocity(jumpVelocityX, jumpVelocityY);
	//pp.setVelocity(0.0f, jumpVelocity);
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void PounceBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	if (currentState != L"DIE"){
		if (cyclesRemainingBeforeThinking == 0)
		{
			//POUNCING LOGIC GOES HERE
			/*if (body->GetLinearVelocity().y == 0)
			{
				GameStateManager *gsm = game->getGSM();
				Jump(gsm->getPhysics());
				resetThinkCycles();
			}*/
		}
		else{
			cyclesRemainingBeforeThinking--;
		}

		animationCounter++;
	}
}