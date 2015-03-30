#pragma once
#include "sssf\gsm\physics\AABB.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"

class Quadtree{
private:
	unsigned int MAX_DEPTH = 3;

	//Depth counter
	unsigned int depth;

	//Checks if tree has been constructed already
	bool constructed = false;

	//List of arrays which correspond to nodes in quadtree
	list<Bot*> bots[21];
	AABB dimensions[21];

public:
	Quadtree(){}
	~Quadtree(){}
	//constructor for child nodes
	Quadtree(Game* game);

	//Get methods
	unsigned int getDepth(){ return depth; }
	list<Bot*> getBots(int index){ return bots[index]; }
	AABB getDimension(unsigned int index){ return dimensions[index]; }
	bool hasParent(int index){ return index != 0; }

	//Functionality methods
	void split(AABB* aabb, unsigned int index, unsigned int depth);
	void assignBoundsToQuadtree(AABB* aabb, unsigned int index);
	int getIndex(AABB* botaabb, unsigned int nodeIndex);
	void insert(Bot* bot, unsigned int nodeIndex, unsigned int depth);
	list<int> insertAABB(AABB* aabb);
	void clear();
};