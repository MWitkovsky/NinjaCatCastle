/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Physics.cpp

	See Physics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\world\SparseLayer.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\WorldLayer.h"
#include <vector>
#include <list>

/*
	The constructor initializes the data structures and loads
	the necessary ones with recyclable objects for collision tests.
*/
Physics::Physics()
{
	//Down is finally negative Y!
	b2Gravity.Set(0.0f, -9.8f);
	//WWWWRRRRRYYYYYYYYYYYYYYYYYY
	world = new b2World(b2Gravity);
}

/*
	The destructor deletes all the recyclable objects.
*/
Physics::~Physics()
{
}

/*
	Informs the physics system of additional dynamic, collidable object
	to keep track of. Note that this method should not be used mid-collision, 
	as a result of a collition event, since it does not update all the
	swept shape positions. Also note that it does not test to see if the 
	added object overlaps an object already in the game.
*/
void Physics::addCollidableObject(CollidableObject *collidableObjectToAdd)
{
	//b2Body *groundBody = world->CreateBody(&collidableObjectToAdd->getBodyDef());
	//collidableObjectToAdd->getShape().SetAsBox(0.0f, 0.0f);
}

/*
	Removes a dynamic, collidable object from the physics system. Note that
	this method should not be used mid-collision, as a result of a collition 
	event, since it does not update all the swept shape positions. Also note 
	that it does not test to see if the added object overlaps an object already 
	in the game.
*/
void Physics::removeCollidableObject(CollidableObject *collidableObjectToRemove)
{
}

/*
	This is where all game physics starts each frame. It is called each frame 
	by the game statem manager after player input and AI have been processed. It
	updates the physical state of all dynamic objects in the game and
	moves all objects to their end of frame positions, updates all necessary
	object velocities, and calls all collision event handlers.
*/
void Physics::update(Game *game)
{
	//No more original physics. I have no idea what to do here for now.
	world->Step(0.1f, 6, 2);
}

void Physics::removeCollidableObject(unsigned int ordering, CollidableObject *co)
{
}