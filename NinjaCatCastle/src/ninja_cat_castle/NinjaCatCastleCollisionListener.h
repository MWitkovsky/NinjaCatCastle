#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\CollisionListener.h"
#include "sssf\gsm\physics\Collision.h"

class NinjaCatCastleCollisionListener : public CollisionListener
{
public:
	void BeginContact(b2Contact* contact);
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	void respondToCollision(AnimatedSprite *player, AnimatedSprite *enemy, b2Contact* contact);
};