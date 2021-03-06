/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\ai\BotRecycler.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"

class SpriteManager
{
private:
	const float		PLAYER_HURTBOX_OFFSET_Y = 0.2f;
	const float		PLAYER_HURTBOX_OFFSET_X = 0.7f;

	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;
	list<AnimatedSprite*> projectiles;
	AnimatedSprite* hpui[3];

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	AnimatedSprite player;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler recyclableBots;

	float meterToPixelScale = 64;

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager()		{}
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	int						getNumberOfSprites()	{ return bots.size();		}
	AnimatedSprite*			getPlayer()				{ return &player;			}
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}
	list<AnimatedSprite*>	getProjectiles()		{ return projectiles;		}
	AnimatedSprite**		getHPUI()				{ return hpui; }

	float					getQuadtreeNodeX(unsigned int index);
	float					getQuadtreeNodeY(unsigned int index);

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addProjectile(AnimatedSprite *projectileToAdd);
	void				addSpriteItemsToRenderList(Game *game);
	void				box2DDebugRender(Game *game, b2Body *body, RenderList *renderList, Viewport *viewport, AnimatedSprite *sprite);
	void				renderVisibleSprite(Game *game, AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(Game *game, AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	void				unloadSprites(Game *game);
	unsigned int		getNumberOfBots();
	unsigned int		getNumberOfBotsInNode(int node);
	//void				checkForCollisions(Game *game, AnimatedSprite *sprite);
	void				update(Game *game);
	void				updateAnimations(Game *game);
};