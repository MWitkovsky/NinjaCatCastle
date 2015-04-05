#include "sssf\gsm\sprite\Quadtree.h"

Quadtree::Quadtree(Game* game){
	depth = 1;
	dimensions[0].setHeight((float)game->getGSM()->getWorld()->getWorldHeight());
	dimensions[0].setWidth((float)game->getGSM()->getWorld()->getWorldWidth());
	dimensions[0].setCenterX(game->getGSM()->getWorld()->getWorldWidth() / 2.0f);
	dimensions[0].setCenterY(game->getGSM()->getWorld()->getWorldHeight() / 2.0f);
	if (depth <= MAX_DEPTH){
		split(&dimensions[0], 0, depth + 1);
	}
	constructed = true;
}

void Quadtree::split(AABB* aabb, unsigned int index, unsigned int depth){
	unsigned int startingIndex = index * 4 + 1;

	float halfWidth = (aabb->getWidth() / 2);
	float halfHeight = (aabb->getHeight() / 2);
	float x = aabb->getCenterX();
	float y = aabb->getCenterY();

	//Set up AABB for sub-quadrants
	dimensions[startingIndex].setCenterX(x - (halfWidth / 2));
	dimensions[startingIndex].setCenterY(y - (halfHeight / 2));
	dimensions[startingIndex].setWidth(halfWidth);
	dimensions[startingIndex].setHeight(halfHeight);

	dimensions[startingIndex + 1].setCenterX(x + (halfWidth / 2));
	dimensions[startingIndex + 1].setCenterY(y - (halfHeight / 2));
	dimensions[startingIndex + 1].setWidth(halfWidth);
	dimensions[startingIndex + 1].setHeight(halfHeight);

	dimensions[startingIndex + 2].setCenterX(x - (halfWidth / 2));
	dimensions[startingIndex + 2].setCenterY(y + (halfHeight / 2));
	dimensions[startingIndex + 2].setWidth(halfWidth);
	dimensions[startingIndex + 2].setHeight(halfHeight);

	dimensions[startingIndex + 3].setCenterX(x + (halfWidth / 2));
	dimensions[startingIndex + 3].setCenterY(y + (halfHeight / 2));
	dimensions[startingIndex + 3].setWidth(halfWidth);
	dimensions[startingIndex + 3].setHeight(halfHeight);

	if (depth != MAX_DEPTH){
		split(&(dimensions[startingIndex]), startingIndex, depth + 1);
		split(&(dimensions[startingIndex + 1]), startingIndex + 1, depth + 1);
		split(&(dimensions[startingIndex + 2]), startingIndex + 2, depth + 1);
		split(&(dimensions[startingIndex + 3]), startingIndex + 3, depth + 1);
	}
}

void Quadtree::assignBoundsToQuadtree(AABB* aabb, unsigned int index){
	this->depth = depth;
	dimensions[index].setCenterX(aabb->getCenterX());
	dimensions[index].setCenterY(aabb->getCenterY());
	dimensions[index].setWidth(aabb->getWidth());
	dimensions[index].setHeight(aabb->getHeight());

	if (depth < MAX_DEPTH){
		split(aabb, 0, depth + 1);
	}
	constructed = true;
}

int Quadtree::getIndex(AABB* botaabb, unsigned int nodeIndex){
	int i = 0;

	unsigned int startingIndex = nodeIndex * 4 + 1;

	bool topLeft = botaabb->overlaps(&(dimensions[startingIndex]));
	bool topRight = botaabb->overlaps(&(dimensions[startingIndex + 1]));
	bool botLeft = botaabb->overlaps(&(dimensions[startingIndex + 2]));
	bool botRight = botaabb->overlaps(&(dimensions[startingIndex + 3]));

	if (topLeft){
		i++;
	}

	if (topRight){
		i++;
	}

	if (botLeft){
		i++;
	}

	if (botRight){
		i++;
	}
	if (i > 1){
		i = -1;
	}
	else{
		if (topLeft){
			i = 1;
		}

		if (topRight){
			i = 2;
		}

		if (botLeft){
			i = 3;
		}

		if (botRight){
			i = 4;
		}
	}

	return i;
}

void Quadtree::insert(Bot* bot, unsigned int nodeIndex, unsigned int depth){
	AABB* botaabb = bot->getBoundingVolume();
	int index = getIndex(botaabb, nodeIndex);

	if (index == -1){
		bots[nodeIndex].push_back(bot);
	}
	else if (depth < MAX_DEPTH){
		unsigned int startingIndex = nodeIndex * 4 + 1;

		if (index == 1){
			insert(bot, startingIndex, depth + 1);
		}
		else if (index == 2){
			insert(bot, startingIndex + 1, depth + 1);
		}
		else if (index == 3){
			insert(bot, startingIndex + 2, depth + 1);
		}
		else{
			insert(bot, startingIndex + 3, depth + 1);
		}
	}
	else{
		bots[nodeIndex].push_back(bot);
	}
}

//Checks each individual division of the map for if the viewport can see it
list<int> Quadtree::insertAABB(AABB* aabb){
	list<int> nodeIndexes;

	for (int i = 0; i < 21; i++){
		if (aabb->overlaps(&(dimensions[i]))){
			nodeIndexes.push_back(i);
		}
	}
	return nodeIndexes;
}

void Quadtree::clear(){

	/*
	for (int i = 0; i < (((pow(4, depth)) - 1) / 3); i++){
	bots[i].clear();
	}
	*/

	for (int i = 0; i < 21; i++){
		bots[i].clear();
	}
}