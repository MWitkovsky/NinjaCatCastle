#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\AABB.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "Box2D\Box2D.h"

// THESE ARE THE SWEEP AND PRUNE ORDERINGS
const unsigned int LEFT_EDGE = 0;
const unsigned int RIGHT_EDGE = 1;
const unsigned int TOP_EDGE = 2;
const unsigned int BOTTOM_EDGE = 3;

class CollidableObject
{
protected:
	//These are replacements for the AABB and physical properties
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	bool onTileThisFrame;
	bool onTileLastFrame;
	unsigned int collisionEdge;

public:
	CollidableObject()	{}
	~CollidableObject()	{}
	virtual void Something() {}

	// INLINED METHODS
	b2BodyDef			getBodyDef()				{ return bodyDef; }
	b2PolygonShape		getShape()					{ return shape; }
	bool				isOnTileThisFrame()			{ return onTileThisFrame;		}
	bool				wasOnTileLastFrame()		{ return onTileLastFrame;		}
	unsigned int		getCollisionEdge()			{ return collisionEdge;			}

	void				setCollisionEdge(unsigned int initCollisionEdge)
	{	collisionEdge = initCollisionEdge; }
	void setOnTileThisFrame(bool initOnTileThisFrame)
	{	onTileThisFrame = initOnTileThisFrame; }
	void setOnTileLastFrame(bool initOnTileLastFrame)
	{	onTileLastFrame = initOnTileLastFrame; }

	void advanceOnTileStatus()
	{
		onTileLastFrame = onTileThisFrame;
		onTileThisFrame = false;
	}

};