#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\CollisionListener.h"
#include "sssf\gsm\physics\Collision.h"

class NinjaCatCastleCollisionListener : public CollisionListener
{
public:
	void BeginContact(b2Contact* contact);
	void respondToCollision(AnimatedSprite *player, AnimatedSprite *enemy);
};