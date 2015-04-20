#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
private:
	boolean hitPlayer = false;
	boolean hitGuard = false;
public:
	Bot()	{}
	~Bot()	{}
	
	boolean didHitPlayer(){ 
		return hitPlayer; 
	}
	boolean hasHitGuard(){
		return hitGuard;
	}
	void setHitPlayer(boolean hitPlayer){
		this->hitPlayer = hitPlayer;
		hitGuard = hitPlayer;
	}
	void setHitGuard(boolean hitGuard){
		this->hitGuard = hitGuard;
	}
	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
};