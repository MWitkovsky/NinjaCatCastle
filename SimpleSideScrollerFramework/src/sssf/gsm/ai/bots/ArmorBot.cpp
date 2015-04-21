#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\ArmorBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

ArmorBot::ArmorBot()
{
	resetThinkCycles();
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void ArmorBot::think(Game *game)
{
	if (!isDead()){
		if (!isAirborne()){
			if (cyclesRemainingBeforeThinking){
				cyclesRemainingBeforeThinking--;
			}
			else{
				GameStateManager *gsm = game->getGSM();
				resetThinkCycles();
			}
		}
	}
	animationCounter++;
}
	