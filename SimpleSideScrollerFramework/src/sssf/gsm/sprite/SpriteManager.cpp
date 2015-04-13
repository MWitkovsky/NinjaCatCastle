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
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Collision.h"
#include "sssf\gsm\sprite\Quadtree.h"
#include "Box2D\Box2D.h"
AABB viewportDimensions;

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(Game *game, AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	//Everything using PP now is using the body
	b2Body *pp = sprite->getBody();
	float32 x = (pp->GetPosition().x)*meterToPixelScale;
	float32 y = (game->getGSM()->getWorld()->getWorldHeight()) - (pp->GetPosition().y*meterToPixelScale);
	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									x,
									y,
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		PounceBot* pounceBot = dynamic_cast<PounceBot*>(sprite);
		if (pounceBot){
			RenderItem itemToAdd;
			itemToAdd.id = sprite->getFrameIndex();
			renderList->addRenderItem(sprite->getCurrentImageID(),
				x - viewport->getViewportX(),
				y - viewport->getViewportY() - 32,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else{
			RenderItem itemToAdd;
			itemToAdd.id = sprite->getFrameIndex();
			renderList->addRenderItem(sprite->getCurrentImageID(),
				x - viewport->getViewportX(),
				y - viewport->getViewportY(),
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
	}
}

void SpriteManager::box2DDebugRender(Game *game, b2Body *body, RenderList *renderList, Viewport *viewport, AnimatedSprite *sprite){
	float32 x = (body->GetPosition().x)*meterToPixelScale;
	float32 y = (game->getGSM()->getWorld()->getWorldHeight()) - (body->GetPosition().y*meterToPixelScale);
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	if (viewport->areWorldCoordinatesInViewport(
		x,
		y,
		spriteType->getTextureWidth(),
		spriteType->getTextureHeight())){
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(sprite->getCurrentImageID(),
			x - viewport->getViewportX(),
			y - viewport->getViewportY(),
			0,
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
		addSpriteToRenderList(game, &player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(game, bot, renderList, viewport);
			botIterator++;
		}

		//Debugging Box2D
		/*b2Body* list = game->getGSM()->getPhysics()->getWorld()->GetBodyList();
		while(list){
			box2DDebugRender(game, list, renderList, viewport, &player);
			list = list->GetNext();
		}*/
	}
}

/*
Uses quad tree for rendering instead of list
*/
/*void SpriteManager::addSpriteItemsToRenderListQuadtree(Game *game){
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		// Since player isn't in Quadtree, use old method
		b2BodyDef *pp = &player.getBodyDef();
		AnimatedSpriteType *spriteType = player.getSpriteType();
		if (viewport->areWorldCoordinatesInViewport(
			pp->position.x,
			pp->position.y,
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

}*/

void SpriteManager::renderVisibleSprite(Game *game, AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport){
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	b2Body *pp = sprite->getBody();
	RenderItem itemToAdd;
	itemToAdd.id = sprite->getFrameIndex();
	renderList->addRenderItem(sprite->getCurrentImageID(),
		(int)(pp->GetPosition().x*meterToPixelScale) - viewport->getViewportX(),
		(int)(game->getGSM()->getWorld()->getWorldHeight())-(pp->GetPosition().y*meterToPixelScale) - viewport->getViewportY(),
		0,
		sprite->getAlpha(),
		spriteType->getTextureWidth(),
		spriteType->getTextureHeight());
}

/*void SpriteManager::placeBotsInTree(){
	botsTree->clear();

	list<Bot*>::iterator botIterator = bots.begin();
	while (botIterator != bots.end()){
		botsTree->insert(*botIterator, 0, 1);
		botIterator++;
	}
}*/

/*list<Bot*> SpriteManager::getVisibleBots(Game *game){
	list<Bot*> visibleBots;

	Viewport *viewport = game->getGUI()->getViewport();

	viewportDimensions.setCenterX((viewport->getViewportWidth() / 2.0f) + viewport->getViewportX());
	viewportDimensions.setCenterY((viewport->getViewportHeight() / 2.0f) + viewport->getViewportY());
	viewportDimensions.setHeight((float)viewport->getViewportHeight());
	viewportDimensions.setWidth((float)viewport->getViewportWidth());

	list<int> nodes = botsTree->insertAABB(&viewportDimensions);
	list<int>::iterator nodeIterator = nodes.begin();

	b2BodyDef* pp;
	AnimatedSpriteType* spriteType;

	list<Bot*> temp;
	while (nodeIterator != nodes.end()){
		temp = botsTree->getBots(*nodeIterator);
		while (!temp.empty()){
			pp = &temp.front()->getBodyDef();
			spriteType = temp.front()->getSpriteType();
			if (viewport->areWorldCoordinatesInViewport(
				pp->position.x,
				pp->position.y,
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight())){
				visibleBots.push_back(temp.front());
			}
			temp.pop_front();
		}
		nodeIterator++;
	}

	return visibleBots;
}*/

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

	//Deletes all bodies from THE WORLD
	b2Body* bodyToDelete = game->getGSM()->getPhysics()->getWorld()->GetBodyList();
	b2Body* temp;
	while (bodyToDelete){
		temp = bodyToDelete->GetNext();
		game->getGSM()->getPhysics()->getWorld()->DestroyBody(bodyToDelete);
		bodyToDelete = temp;
	}
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}

unsigned int SpriteManager::getNumberOfBots(){
	return bots.size();
}

/*unsigned int SpriteManager::getNumberOfBotsInNode(int node){
	return botsTree->getBots(node).size();
}*/

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	//Player stuff mostly
	updateAnimations(game);

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

	//legacy
	//checkForCollisions(game, &player);
}

void SpriteManager::updateAnimations(Game *game){
	// UPDATE THE PLAYER SPRITE
	float velocityY = player.getBody()->GetLinearVelocity().y;
	wstring state = player.getCurrentState();
	if (velocityY < 0){
		if (player.wasHit()){
			player.setHit(false);
		}
	}

	//ANIMATION STUFF
	if (state != L"ATTACK_RIGHT_1" && state != L"ATTACK_RIGHT_2"
		&& state != L"ATTACK_LEFT_1" && state != L"ATTACK_LEFT_2"){
		if (velocityY > 0){
			player.setAirborne(true);
			if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT" || state == L"JUMPING_ASCEND_RIGHT"){
				player.setCurrentState(L"JUMPING_ASCEND_RIGHT");
			}
			else if (state == L"WALK_LEFT" || state == L"IDLE_LEFT" || state == L"JUMPING_ASCEND_LEFT"){
				player.setCurrentState(L"JUMPING_ASCEND_LEFT");
			}
		}
		else if (velocityY < 0.0f && state == L"JUMPING_ASCEND_LEFT"){
			player.setAirborne(true);
			player.setCurrentState(L"JUMPING_ARC_LEFT");
		}
		else if (velocityY < 0.0f && state == L"JUMPING_ASCEND_RIGHT"){
			player.setAirborne(true);
			player.setCurrentState(L"JUMPING_ARC_RIGHT");
		}
		else if (velocityY < 0.0f && (state == L"WALK_LEFT" || state == L"IDLE_LEFT")){
			player.setAirborne(true);
			player.setCurrentState(L"JUMPING_DESCEND_LEFT");
		}
		else if (velocityY < 0.0f && (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT")){
			player.setAirborne(true);
			player.setCurrentState(L"JUMPING_DESCEND_RIGHT");
		}
		else if (velocityY == 0.0f && state != L"JUMPING_ASCEND_LEFT" && state != L"JUMPING_ASCEND_RIGHT"
			&& state != L"JUMPING_ARC_LEFT" && state != L"JUMPING_ARC_RIGHT"){
			if (!player.wasHit()){
				player.setAirborne(false);
				if (state == L"JUMPING_DESCEND_LEFT" || state == L"HIT_RIGHT"){
					player.setCurrentState(L"IDLE_LEFT");
				}
				else if ((state == L"HIT_LEFT" || state == L"HIT_RIGHT") && player.getHP() == 0){
					player.setCurrentState(L"DIE");
				}
				else if (player.getCurrentState() == L"JUMPING_DESCEND_RIGHT" || state == L"HIT_LEFT"){
					player.setCurrentState(L"IDLE_RIGHT");
				}
			}
		}
	}

	if (player.isAttackFinished()){
		if (player.getHurtBox()){
			game->getGSM()->getPhysics()->getWorld()->DestroyBody(player.getHurtBox());
			player.setHurtBox(NULL);
			player.setAttackFinished(false);
			player.setAttacking(false);
		}
	}

	if (player.getHurtBox()){
		float32 y = player.getBody()->GetPosition().y;
		float32 x;
		if (player.isFacingRight()){
			x = player.getBody()->GetPosition().x + 0.7f;
		}
		else{
			x = player.getBody()->GetPosition().x - 0.7f;
		}
		
		player.getHurtBox()->SetTransform(b2Vec2(x, y), player.getHurtBox()->GetAngle());
	}
	
	//Invincibility frames should ideally last about 1 second.
	if (player.getInvincibilityFrames() != 0){
		player.decrementInvincibilityFrames();
	}

	//Makes it so if the player falls off a platform they can't jump afterwards
	if (player.getBody()->GetLinearVelocity().y == 0){
		player.setWasJump(false);
	}

	//LEGACY FLICKER CODE FROM HOMEWORK 3
	/*if (player.getHP() != 10){
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
	}*/

	player.updateSprite();
}