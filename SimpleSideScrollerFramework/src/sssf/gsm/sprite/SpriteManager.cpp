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
#include "sssf\gsm\ai\bots\PropellerBot.h"
#include "sssf\gsm\ai\bots\BombBot.h"
#include "sssf\gsm\ai\bots\ArmorBot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Collision.h"
#include "Box2D\Box2D.h"

AABB viewportDimensions;
int waitTimer = 120;
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
									x+16,
									y,
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight())
									|| viewport->areWorldCoordinatesInViewport(
									x,
									y,
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		if (dynamic_cast<PounceBot*>(sprite)){
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX(),
				(int)y - viewport->getViewportY() - 32,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else if (dynamic_cast<ArmorBot*>(sprite)){
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX() - 64,
				(int)y - viewport->getViewportY() - 96,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else if(sprite->getSpriteType()->getSpriteTypeID() == 2){ //SHURIKEN OFFSETS
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX()+64,
				(int)y - viewport->getViewportY()+64,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else if (sprite->getSpriteType()->getSpriteTypeID() == 7){ //Health pickups
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX()+48,
				(int)y - viewport->getViewportY()+16,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else if (sprite->getSpriteType()->getSpriteTypeID() == 8){ //Treat pickups
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX() + 44,
				(int)y - viewport->getViewportY() + 54,
				0,
				sprite->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
		else{
			renderList->addRenderItem(sprite->getCurrentImageID(),
				(int)x - viewport->getViewportX(),
				(int)y - viewport->getViewportY(),
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
			(int)x - viewport->getViewportX(),
			(int)y - viewport->getViewportY(),
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

		//Debugging Box2D
		/*b2Body* bodies = game->getGSM()->getPhysics()->getWorld()->GetBodyList();
		while (bodies){
			box2DDebugRender(game, bodies, renderList, viewport, &player);
			bodies = bodies->GetNext();
		}*/

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(game, &player, renderList, viewport);

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = *botIterator;
			if (bot->getBody()){
				addSpriteToRenderList(game, bot, renderList, viewport);
			}
			botIterator++;
		}

		list<AnimatedSprite*>::iterator projectileIterator;
		projectileIterator = projectiles.begin();
		boolean activeProjectileExists = false;
		while (projectileIterator != projectiles.end()){
			AnimatedSprite *projectile = *projectileIterator;
			if (projectile->getBody()){
				addSpriteToRenderList(game, projectile, renderList, viewport);
				if (projectile->getCurrentState() != L"EXPLODE2"){
					activeProjectileExists = true;
				}
			}
			projectileIterator++;
		}
		if (!activeProjectileExists && projectiles.size() > 0){
			projectiles.clear();
		}

		for (int i = 0; i < 3; i++){
			AnimatedSpriteType *spriteType = hpui[i]->getSpriteType();
			renderList->addRenderItem(hpui[i]->getCurrentImageID(),
				15 + (75*i),
				SHURIKEN_LABEL_Y,
				0,
				hpui[i]->getAlpha(),
				spriteType->getTextureWidth(),
				spriteType->getTextureHeight());
		}
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
		(int)(game->getGSM()->getWorld()->getWorldHeight())-(int)(pp->GetPosition().y*meterToPixelScale) - viewport->getViewportY(),
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

void SpriteManager::addProjectile(AnimatedSprite *projectileToAdd){
	projectiles.push_back(projectileToAdd);
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
	projectiles.clear();
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
	bots.clear();
	projectiles.clear();
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

	//Some sound handling that couldn't be done where I wanted it to
	if (player.getTreats() >= 100){
		player.setLives(player.getLives() + 1);
		player.setTreats(player.getTreats() - 100);
		//Play sound here. Either a special sound effect like Mario or even a whole jingle like Sonic.
		introChannel = game->playOverlappingJingle(ONE_UP_JINGLE, introChannel, 240);
		musicChannel->setPaused(true);
	}

	if (player.shouldPlayTreatSound()){
		game->playSound(SOUND_PICKUP_TREAT);
		player.setShouldPlayTreatSound(false);
	}

	if (player.shouldPlayHealthSound()){
		game->playSound(SOUND_PICKUP_HEALTH);
		player.setShouldPlayHealthSound(false);
	}

	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		bot->think(game);
		bot->updateSprite(game);
		botIterator++;
	}

	list<AnimatedSprite*>::iterator projectileIterator;
	projectileIterator = projectiles.begin();
	while (projectileIterator != projectiles.end()){
		AnimatedSprite *projectile = *projectileIterator;
		if (projectile->isProjectile()){
			projectile->updateSprite(game);
		
			if (projectile->getCurrentState() != L"EXPLODE" && projectile->getCurrentState() != L"COUNTDOWN"
				&& projectile->getCurrentState() != L"EXPLODE2"){
				if (projectile->getBody()->GetLinearVelocity().x == 0.0f
					|| projectile->getBody()->GetLinearVelocity().y == 0.0f
					|| projectile->isMarkedForDeletion()){
					if (projectile->getBody()){
						game->getGSM()->getPhysics()->getWorld()->DestroyBody(projectile->getBody());
						projectile->setIsProjectile(false);
						projectile->setBody(NULL);
					}
					if (projectile->isMarkedForDeletion()){
						projectile->markForDeletion(false);
					}
				}
			}
			
		}
			
		projectileIterator++;
	}
}

void SpriteManager::updateAnimations(Game *game){
	// UPDATE THE PLAYER SPRITE
	float velocityY = player.getBody()->GetLinearVelocity().y;
	wstring state = player.getCurrentState();
	if (player.getBody()->GetLinearVelocity().y < 0.00000000000001f && player.getBody()->GetLinearVelocity().y > 0.0f){
		player.getBody()->SetLinearVelocity(b2Vec2(player.getBody()->GetLinearVelocity().x, 0.0f));
	}
	if (velocityY == 0){
		if (player.hasAirborneGuard()){
			player.setAirborneGuard(false);
		}
		else{
			player.setAirborne(false);
			if (player.getBody()->GetFixtureList()){
				player.getBody()->DestroyFixture(player.getBody()->GetFixtureList());
			}
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			float32 width = 0.5f;
			float32 height = 0.8f;
			float32 edgeWidth = 0.0f;
			float32 edgeHeight = 0.0f;
			b2Vec2 vertices[8];
			vertices[0].Set(-width + edgeWidth, -height);		// bottom
			vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
			vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
			vertices[3].Set(width, height - edgeHeight);		// top-right edge start
			vertices[4].Set(width - edgeWidth, height);			// top-right edge end
			vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
			vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
			vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
			shape.Set(vertices, 8);
			fixtureDef.shape = &shape;
			player.getBody()->CreateFixture(&fixtureDef);

			player.getBody()->GetFixtureList()->SetFriction(1.0f);
			if (player.wasHit()){
				player.setHit(false);
			}
		}
	}
	else{
		player.setAirborne(true);
		player.getBody()->GetFixtureList()->SetFriction(0.0f);
	}

	//Checking if level is completed
	if (player.getBody()->GetPosition().x * 64 > game->getGSM()->getWorld()->getWorldWidth()){
		if (waitTimer == 120){
			musicChannel = game->playSongIntro(LEVEL_COMPLETE_JINGLE, musicChannel);
			player.getBody()->SetGravityScale(0.0f);
			player.getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			waitTimer--;
			player.setControllable(false);
		}
		else if (waitTimer < 120 && waitTimer > 0){
			waitTimer--;
		}
		else{
			waitTimer = 120;
			player.getBody()->SetGravityScale(1.0f);
			player.setControllable(true);
			if (game->getCurrentLevelFileName() == W_LEVEL_1_NAME){
				game->quitGame();
				game->setCurrentLevelFileName(W_LEVEL_2_NAME);
				game->startGame();
			}
			else if (game->getCurrentLevelFileName() == W_LEVEL_2_NAME){
				game->quitGame();
				game->setCurrentLevelFileName(W_LEVEL_3_NAME);
				game->startGame();
			}
			else{
				player.setLives(3);
				game->quitGame();
			}
		}
	}

	//ANIMATION STUFF
	if (!player.isAttacking()){
		if (player.isProjectile()){
			player.setIsProjectile(false);
			if (player.getBody()->GetFixtureList()){
				player.getBody()->DestroyFixture(player.getBody()->GetFixtureList());
			}
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			float32 width = 0.5f;
			float32 height = 0.8f;
			float32 edgeWidth = 0.0f;
			float32 edgeHeight = 0.0f;
			b2Vec2 vertices[8];
			vertices[0].Set(-width + edgeWidth, -height);		// bottom
			vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
			vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
			vertices[3].Set(width, height - edgeHeight);		// top-right edge start
			vertices[4].Set(width - edgeWidth, height);			// top-right edge end
			vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
			vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
			vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
			shape.Set(vertices, 8);
			fixtureDef.shape = &shape;
			player.getBody()->CreateFixture(&fixtureDef);
			if (player.isFacingRight()){
				player.setCurrentState(L"HIT_LEFT");
				player.getBody()->SetLinearVelocity(b2Vec2(-3.0f, 8.0f));
			}
			else{
				player.setCurrentState(L"HIT_RIGHT");
				player.getBody()->SetLinearVelocity(b2Vec2(3.0f, 8.0f));
			}
			game->playSound(SOUND_HIT2);
		}
		else if (velocityY > 0){
			if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT" || state == L"JUMPING_ASCEND_RIGHT"
				|| state == L"ATTACK_RIGHT" || state == L"ATTACK_RIGHT_2"){
				player.setCurrentState(L"JUMPING_ASCEND_RIGHT");
			}
			else if (state == L"WALK_LEFT" || state == L"IDLE_LEFT" || state == L"JUMPING_ASCEND_LEFT"
				|| state == L"ATTACK_LEFT" || state == L"ATTACK_LEFT_2"){
				player.setCurrentState(L"JUMPING_ASCEND_LEFT");
			}
		}
		else if (velocityY < 0.0f && (state == L"JUMPING_ASCEND_LEFT"
			|| state == L"ATTACK_LEFT" || state == L"ATTACK_LEFT_2")){
			player.setCurrentState(L"JUMPING_ARC_LEFT");
		}
		else if (velocityY < 0.0f && (state == L"JUMPING_ASCEND_RIGHT"
			|| state == L"ATTACK_RIGHT" || state == L"ATTACK_RIGHT_2")){
			player.setCurrentState(L"JUMPING_ARC_RIGHT");
		}
		else if (velocityY < 0.0f && (state == L"WALK_LEFT" || state == L"IDLE_LEFT"
			|| state == L"ATTACK_LEFT" || state == L"ATTACK_LEFT_2")){
			if (player.getBody()->GetFixtureList()){
				player.getBody()->DestroyFixture(player.getBody()->GetFixtureList());
			}
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			float32 width = 0.5f;
			float32 height = 0.8f;
			float32 edgeWidth = 0.0f;
			float32 edgeHeight = 0.0f;
			b2Vec2 vertices[8];
			vertices[0].Set(-width + edgeWidth, -height);		// bottom
			vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
			vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
			vertices[3].Set(width, height - edgeHeight);		// top-right edge start
			vertices[4].Set(width - edgeWidth, height);			// top-right edge end
			vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
			vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
			vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
			shape.Set(vertices, 8);
			fixtureDef.shape = &shape;
			fixtureDef.friction = 0.0f;
			player.getBody()->CreateFixture(&fixtureDef);
			player.setCurrentState(L"JUMPING_DESCEND_LEFT");
		}
		else if (velocityY < 0.0f && (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT"
			|| state == L"ATTACK_RIGHT" || state == L"ATTACK_RIGHT_2")){
			if (player.getBody()->GetFixtureList()){
				player.getBody()->DestroyFixture(player.getBody()->GetFixtureList());
			}
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			float32 width = 0.5f;
			float32 height = 0.8f;
			float32 edgeWidth = 0.0f;
			float32 edgeHeight = 0.0f;
			b2Vec2 vertices[8];
			vertices[0].Set(-width + edgeWidth, -height);		// bottom
			vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
			vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
			vertices[3].Set(width, height - edgeHeight);		// top-right edge start
			vertices[4].Set(width - edgeWidth, height);			// top-right edge end
			vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
			vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
			vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
			shape.Set(vertices, 8);
			fixtureDef.shape = &shape;
			fixtureDef.friction = 0.0f;
			player.getBody()->CreateFixture(&fixtureDef);
			player.setCurrentState(L"JUMPING_DESCEND_RIGHT");
		}
		else if (velocityY == 0.0f){
			if (!player.wasHit()){
				if ((state == L"HIT_LEFT" || state == L"HIT_RIGHT") && player.getHP() <= 0){
					player.getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					if (state == L"HIT_LEFT"){
						player.setCurrentState(L"DIE_RIGHT");
						player.setControllable(false);
						musicChannel->stop();
						game->playSound(SOUND_MC_DEATH);
					}
					else{
						player.setCurrentState(L"DIE_LEFT");
						player.setControllable(false);
						musicChannel->stop();
						game->playSound(SOUND_MC_DEATH);
					}
				}
				else if (state == L"JUMPING_DESCEND_LEFT" || state == L"HIT_RIGHT"
					|| state == L"ATTACK_LEFT" || state == L"ATTACK_LEFT_2"){
					player.setCurrentState(L"LAND_LEFT");
					game->playSound(SOUND_MC_JUMP);
				}
				else if (player.getCurrentState() == L"JUMPING_DESCEND_RIGHT" || state == L"HIT_LEFT"
					|| state == L"ATTACK_RIGHT" || state == L"ATTACK_RIGHT_2"){
					player.setCurrentState(L"LAND_RIGHT");
					game->playSound(SOUND_MC_JUMP);
				}
			}
		}
	}

	if (player.isAttackFinished()){
		player.getHurtBox()->SetActive(false);
		player.setAttacking(false);
	}

	if (player.getHurtBox()->IsActive()){
		float32 y = player.getBody()->GetPosition().y - PLAYER_HURTBOX_OFFSET_Y;
		float32 x;
		if (player.isFacingRight()){
			x = player.getBody()->GetPosition().x + PLAYER_HURTBOX_OFFSET_X;
		}
		else{
			x = player.getBody()->GetPosition().x - PLAYER_HURTBOX_OFFSET_X;
		}
		
		player.getHurtBox()->SetTransform(b2Vec2(x, y), player.getHurtBox()->GetAngle());
	}
	
	//Invincibility frames should ideally last about 2 seconds.
	if (!player.wasHit() && state != L"DIE_LEFT" && state != L"DIE_RIGHT"){
		if (player.getInvincibilityFrames() != 0){
			player.decrementInvincibilityFrames();
			if (player.getInvincibilityFrames() % 15 <= 1 && player.getInvincibilityFrames() != 0){
				player.setAlpha(0);
			}
			else{
				player.setAlpha(255);
			}
		}
	}

	//Makes it so if the player falls off a platform they can't jump afterwards
	if (player.getBody()->GetLinearVelocity().y == 0){
		//So the player can't cancel y velocity by mashing space bar
		player.setWasJump(false);
	}

	//ENEMY STUFF
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end()){
		Bot* genericBot = *botIterator;
		PounceBot* pounceBot = dynamic_cast<PounceBot*>(*botIterator);
		PropellerBot* propellerBot = dynamic_cast<PropellerBot*>(*botIterator);
		BombBot* bombBot = dynamic_cast<BombBot*>(*botIterator);
		ArmorBot* armorBot = dynamic_cast<ArmorBot*>(*botIterator);
		//the propeller cat, since he is always airborne, needs his own state handling
		if (!propellerBot){
			if (genericBot->getBody()->GetLinearVelocity().y == 0){
				if (genericBot->hasAirborneGuard()){
					genericBot->setAirborneGuard(false);
				}
				else{
					genericBot->setAirborne(false);
					genericBot->setHit(false);
				}
			}
			else{
				genericBot->setAirborne(true);
			}
		}
		else{
			//propeller cat state logic goes here
			if (propellerBot->didHitPlayer()){
				if (!propellerBot->hasHitGuard()){
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;
					shape.SetAsBox(0.5f, 0.5f);
					fixtureDef.shape = &shape;
					propellerBot->getBody()->CreateFixture(&fixtureDef);
					propellerBot->setHitPlayer(false);
				}
				else{
					if (propellerBot->getBody()->GetFixtureList()){
						propellerBot->getBody()->DestroyFixture(propellerBot->getBody()->GetFixtureList());
						propellerBot->getBody()->SetLinearVelocity(propellerBot->getTrajectory());
					}
					propellerBot->setHitGuard(false);
				}
			}

			//gravity scale equals 1.0f only if it is in the process of dying!
			if (propellerBot->getBody()->GetGravityScale() == 1.0f){
				if (propellerBot->getBody()->GetLinearVelocity().y == 0){
					if (propellerBot->hasAirborneGuard()){
						propellerBot->setAirborneGuard(false);
					}
					else{
						propellerBot->setAirborne(false);
						propellerBot->setHit(false);
					}
				}
				else{
					propellerBot->setAirborne(true);
				}

				if (!propellerBot->wasHit()){
					propellerBot->getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					if (propellerBot->getCurrentState() == L"HIT_LEFT"){
						propellerBot->setCurrentState(L"DIE_LEFT");
						game->playSound(SOUND_BOMB_EXPLOSION1);
						game->playSound(SOUND_BOMB_EXPLOSION2);
					}
					else if (propellerBot->getCurrentState() == L"HIT_RIGHT"){
						propellerBot->setCurrentState(L"DIE_RIGHT");
						game->playSound(SOUND_BOMB_EXPLOSION1);
						game->playSound(SOUND_BOMB_EXPLOSION2);
					}
				}
			}
			else if (propellerBot->getBody()->GetLinearVelocity().x > 0.0f){
				propellerBot->setFacingRight(true);
			}
			else{
				propellerBot->setFacingRight(false);
			}
		}

		if (genericBot->shouldPlayHitSound()){
			game->playSound(SOUND_HIT);
			genericBot->setPlayHitSound(false);
		}

		if (pounceBot){
			wstring state = pounceBot->getCurrentState();
			if (!pounceBot->isDead()){
				if (pounceBot->didHitPlayer()){
					if (!pounceBot->hasHitGuard()){
						b2FixtureDef fixtureDef;
						b2PolygonShape shape;
						shape.SetAsBox(0.7f, 0.4f);
						fixtureDef.shape = &shape;
						pounceBot->getBody()->CreateFixture(&fixtureDef);
						pounceBot->setHitPlayer(false);
					}
					else{
						if (pounceBot->getBody()->GetFixtureList()){
							pounceBot->getBody()->DestroyFixture(pounceBot->getBody()->GetFixtureList());
							pounceBot->getBody()->SetLinearVelocity(pounceBot->getTrajectory());
						}
						pounceBot->setHitGuard(false);
					}
				}

				if (state == L"HIT_LEFT" || state == L"HIT_RIGHT"){
					if (pounceBot->getBody()->GetLinearVelocity().y == 0){
						if (!pounceBot->wasHit()){
							pounceBot->getBody()->SetSleepingAllowed(true);
							if (pounceBot->getBody()->GetFixtureList()){
								pounceBot->getBody()->DestroyFixture(pounceBot->getBody()->GetFixtureList());
							}
							b2FixtureDef fixtureDef;
							b2PolygonShape shape;
							shape.SetAsBox(0.7f, 0.4f);
							fixtureDef.shape = &shape;
							pounceBot->getBody()->CreateFixture(&fixtureDef);
							pounceBot->getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
							if (state == L"HIT_LEFT"){
								pounceBot->setCurrentState(L"DIE_LEFT");
							}
							else if (state == L"HIT_RIGHT"){
								pounceBot->setCurrentState(L"DIE_RIGHT");
							}
							game->playSound(SOUND_HIT);
						}
					}
				}
				else if (!pounceBot->isAirborne() && pounceBot->getBody()->GetLinearVelocity().x == 0.0f){
					if (state == L"JUMPING_LEFT"){
						pounceBot->Jump(false);
					}
					else if (state == L"JUMPING_RIGHT"){
						pounceBot->Jump(true);
					}
				}
				else if (pounceBot->isAirborne() && pounceBot->getBody()->GetLinearVelocity().x == 0.0f){
					if (pounceBot->getBody()->GetFixtureList()){
						pounceBot->getBody()->DestroyFixture(pounceBot->getBody()->GetFixtureList());
					}
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;
					shape.SetAsBox(0.7f, 0.4f);
					fixtureDef.shape = &shape;
					pounceBot->getBody()->CreateFixture(&fixtureDef);
					if (state == L"JUMPING_LEFT"){
						pounceBot->setCurrentState(L"IDLE_LEFT");
					}
					else if (state == L"JUMPING_RIGHT"){
						pounceBot->setCurrentState(L"IDLE_RIGHT");
					}
				}
			}
		}
		else if (bombBot){
			if (bombBot->isMarkedForDeletion()){
				if (bombBot->getBody()->GetFixtureList()){
					bombBot->getBody()->DestroyFixture(bombBot->getBody()->GetFixtureList());
				}
				bombBot->markForDeletion(false);
			}
			else{
				if (!bombBot->getBody()->GetFixtureList()){
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;

					shape.SetAsBox(0.5f, 0.5f);
					fixtureDef.shape = &shape;

					bombBot->getBody()->CreateFixture(&fixtureDef);
				}
			}
		}
		else if (armorBot){
			if (armorBot->isMarkedForDeletion()){
				if (armorBot->getBody()->GetFixtureList()){
					armorBot->getBody()->DestroyFixture(armorBot->getBody()->GetFixtureList());
				}
				armorBot->markForDeletion(false);
			}
			else{
				if (!armorBot->getBody()->GetFixtureList()){
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;

					shape.SetAsBox(0.5f, 1.5f);
					fixtureDef.shape = &shape;

					armorBot->getBody()->CreateFixture(&fixtureDef);
				}
			}

			if (armorBot->shouldPlayBlockSound()){
				game->playSound(SOUND_ARMORCAT_BLOCK);
				armorBot->setPlayBlockSound(false);
			}

			if (armorBot->shouldPlayDeathSound()){
				game->playSound(SOUND_ARMORCAT_DIE);
				armorBot->setPlayDeathSound(false);
			}
		}
		botIterator++;
	}

	//update heart UI
	for (int i = 0; i < 3; i++){
		if (player.getHP() > i){
			hpui[i]->setCurrentState(L"FULL");
		}
		else{
			hpui[i]->setCurrentState(L"EMPTY");
		}
	}

	player.updateSprite(game);
}