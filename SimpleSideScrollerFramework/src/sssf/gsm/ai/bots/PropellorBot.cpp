#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\PropellorBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
PropellorBot::PropellorBot()
{
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void PropellorBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	if (cyclesRemainingBeforeThinking == 0)
	{
		GameStateManager *gsm = game->getGSM();
	}
	else{
		cyclesRemainingBeforeThinking--;
	}
}