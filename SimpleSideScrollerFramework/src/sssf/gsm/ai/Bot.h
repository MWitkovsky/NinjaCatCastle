#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
private:
	boolean hitPlayer = false;

public:
	Bot()	{}
	~Bot()	{}
	
	boolean didHitPlayer(){ 
		return hitPlayer; 
	}
	void setHitPlayer(boolean hitPlayer){
		this->hitPlayer = hitPlayer;
	}

	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
};