#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"

class CollisionListener
{
public:
	virtual void respondToCollision(Collision *collision)=0;
};