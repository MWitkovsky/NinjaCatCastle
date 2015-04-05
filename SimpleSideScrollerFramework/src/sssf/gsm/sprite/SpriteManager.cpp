/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\sprite\Quadtree.h"

Quadtree* botsTree;
AABB viewportDimensions;

float SpriteManager::getQuadtreeNodeX(unsigned int index){
	return botsTree->getDimension(index).getCenterX();
}

float SpriteManager::getQuadtreeNodeY(unsigned int index){
	return botsTree->getDimension(index).getCenterY();
}

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									pp->getX(),
									pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									pp->round(pp->getX()-viewport->getViewportX()),
									pp->round(pp->getY()-viewport->getViewportY()),
									pp->round(pp->getZ()),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(&player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}
	}
}

/*
Uses quad tree for rendering instead of list
*/
void SpriteManager::addSpriteItemsToRenderListQuadtree(Game *game){
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		// Since player isn't in Quadtree, use old method
		PhysicalProperties *pp = player.getPhysicalProperties();
		AnimatedSpriteType *spriteType = player.getSpriteType();
		if (viewport->areWorldCoordinatesInViewport(
			pp->getX(),
			pp->getY(),
			spriteType->getTextureWidth(),
			spriteType->getTextureHeight()))
			renderVisibleSprite(&player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		if (botsTree == 0){
			botsTree = new Quadtree(game);
		}
		placeBotsInTree();
		list<Bot*> visibleBots = getVisibleBots(game);
		list<Bot*>::iterator visibleBotIterator = visibleBots.begin();
		while (visibleBotIterator != visibleBots.end())
		{
			Bot *bot = (*visibleBotIterator);
			renderVisibleSprite(bot, renderList, viewport);
			visibleBotIterator++;
		}
	}

}

void SpriteManager::renderVisibleSprite(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport){
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();
	RenderItem itemToAdd;
	itemToAdd.id = sprite->getFrameIndex();
	renderList->addRenderItem(sprite->getCurrentImageID(),
		pp->round(pp->getX() - viewport->getViewportX()),
		pp->round(pp->getY() - viewport->getViewportY()),
		pp->round(pp->getZ()),
		sprite->getAlpha(),
		spriteType->getTextureWidth(),
		spriteType->getTextureHeight());
}

void SpriteManager::placeBotsInTree(){
	botsTree->clear();

	list<Bot*>::iterator botIterator = bots.begin();
	while (botIterator != bots.end()){
		botsTree->insert(*botIterator, 0, 1);
		botIterator++;
	}
}

list<Bot*> SpriteManager::getVisibleBots(Game *game){
	list<Bot*> visibleBots;

	Viewport *viewport = game->getGUI()->getViewport();

	viewportDimensions.setCenterX((viewport->getViewportWidth() / 2) + viewport->getViewportX());
	viewportDimensions.setCenterY((viewport->getViewportHeight() / 2) + viewport->getViewportY());
	viewportDimensions.setHeight(viewport->getViewportHeight());
	viewportDimensions.setWidth(viewport->getViewportWidth());

	list<int> nodes = botsTree->insertAABB(&viewportDimensions);
	list<int>::iterator nodeIterator = nodes.begin();

	PhysicalProperties* pp;
	AnimatedSpriteType* spriteType;

	list<Bot*> temp;
	while (nodeIterator != nodes.end()){
		temp = botsTree->getBots(*nodeIterator);
		while (!temp.empty()){
			pp = temp.front()->getPhysicalProperties();
			spriteType = temp.front()->getSpriteType();
			if (viewport->areWorldCoordinatesInViewport(
				pp->getX(),
				pp->getY(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight())){
				visibleBots.push_back(temp.front());
			}
			temp.pop_front();
		}
		nodeIterator++;
	}

	return visibleBots;
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites(Game *game)
{
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT

	/*list<Bot*>::iterator botsIt = bots.begin();
	while (botsIt != bots.end())
	{
		list<Bot*>::iterator tempIt = botsIt;
		botsIt++;
		Bot *botToDelete = (*tempIt);
		CollidableObject *botCO = (CollidableObject*)botToDelete;
		game->getGSM()->getPhysics()->removeCollidableObject(botCO);
		delete botToDelete;
	}*/

	/*for each(Bot* botToDelete in bots){
		delete botToDelete;
	}
	bots.clear();*/
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}

unsigned int SpriteManager::getNumberOfBots(){
	return bots.size();
}

unsigned int SpriteManager::getNumberOfBotsInNode(int node){
	return botsTree->getBots(node).size();
}

void SpriteManager::checkForCollisions(Game *game, AnimatedSprite *player){
	list<int> nodesToCheck = botsTree->insertAABB(player->getSweptShape());
	list<int>::iterator nodeIterator = nodesToCheck.begin();

	if (!(*player).getBoundingVolume()->overlaps(&botsTree->getDimension(0))){
		game->getGSM()->unloadCurrentLevel(game);
		game->setCurrentLevelFileName(L"SideScrollerDesertLevel.tmx");
		list<Bot*>::iterator botsIt = bots.begin();
		while (botsIt != bots.end()){
			(*botsIt)->getPhysicalProperties()->setOriginalY(100);
			botsIt++;
		}
		game->getDataLoader()->loadWorld(game, L"data/levels/SideScrollerDesert/", L"SideScrollerDesertLevel.tmx");
	}

	list<Bot*> temp;
	Collision collision;
	collision.setCollisionWithTile(false);
	collision.setCO1(player);
	while (nodeIterator != nodesToCheck.end()){
		temp = botsTree->getBots(*nodeIterator);
		while (!temp.empty()){
			if (temp.front()->getSweptShape()->overlaps(player->getSweptShape())){
				collision.setCO2(temp.front());
				game->getGSM()->getPhysics()->getCollisionListener()->respondToCollision(&collision);
			}
			temp.pop_front();
		}
		nodeIterator++;
	}
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	// UPDATE THE PLAYER SPRITE
	int velocityY = player.getPhysicalProperties()->getVelocityY();
	wstring state = player.getCurrentState();
	if (velocityY < 0){
		if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT" || state == L"JUMPING_ASCEND_RIGHT"){
			player.setCurrentState(L"JUMPING_ASCEND_RIGHT");
		}
		else if (state == L"WALK_LEFT" || state == L"IDLE_LEFT" || state == L"JUMPING_ASCEND_LEFT"){
			player.setCurrentState(L"JUMPING_ASCEND_LEFT");
		}
	}
	else if (velocityY > 0 && state == L"JUMPING_ASCEND_LEFT"){
		player.setCurrentState(L"JUMPING_ARC_LEFT");
	}
	else if (velocityY > 0 && state == L"JUMPING_ASCEND_RIGHT"){
		player.setCurrentState(L"JUMPING_ARC_RIGHT");
	}
	else if (velocityY > 0 && (state == L"WALK_LEFT" || state == L"IDLE_LEFT")){
		player.setCurrentState(L"JUMPING_DESCEND_LEFT");
	}
	else if (velocityY > 0 && (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT")){
		player.setCurrentState(L"JUMPING_DESCEND_RIGHT");
	}
	else if (velocityY == 0 && state != L"JUMPING_ASCEND_LEFT" && state != L"JUMPING_ASCEND_RIGHT"
		&& state != L"JUMPING_ARC_LEFT" && state != L"JUMPING_ARC_RIGHT"){
		if (player.getCurrentState() == L"JUMPING_DESCEND_LEFT"){
			player.setCurrentState(L"IDLE_LEFT");
		}
		else if (state == L"HIT" && player.getHP() == 0){
			player.setCurrentState(L"DIE");
		}
		else if (player.getCurrentState() == L"JUMPING_DESCEND_RIGHT" || state == L"HIT"){
			player.setCurrentState(L"IDLE_RIGHT");
		}
		
	}
	player.updateSprite();

	if (player.getInvincibilityFrames() != 0){
		player.decrementInvincibilityFrames();
	}
	if (player.isOnTileThisFrame()){
		player.setCanDoubleJump(false);
	}
	if (player.getHP() != 10){
		if (player.getHP() == 0){
			player.setAlpha(255);
			if (player.getVisibleFrames() == 0){
				player.setVisibleFrames(10);
				if (player.getLives() > 1){
					player.decrementLives();
					game->quitGame();
					game->getGSM()->goToLoadLevel();
					game->startGame();
				}
				else{
					player.setLives(3);
					game->quitGame();
				}
			}
			else{
				player.decrementVisibleFrames();
			}
		}
		else{
			if (player.getVisibleFrames() <= 0){
				player.setAlpha(0);
				player.setVisibleFrames(player.getHP());
			}
			else{
				player.setAlpha(255);
				player.decrementVisibleFrames();
			}
		}
	}
	else{
		player.setAlpha(255);
	}

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		bot->think(game);
		bot->updateSprite();
		botIterator++;
	}

	checkForCollisions(game, &player);
}