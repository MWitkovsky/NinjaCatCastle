#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "Box2D\Box2D.h"

class CollisionListener : public b2ContactListener
{
public:
	virtual void respondToCollision(AnimatedSprite *player, AnimatedSprite *enemy, b2Contact* contact) = 0;
};