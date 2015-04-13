/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gui\Viewport.h"

class AnimatedSprite : public CollidableObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;
	wstring originalState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	// Player Specific values
	boolean player = false;
	boolean controllable = true;
	boolean jump = false;
	boolean facingRight = false;
	boolean hit = false;
	boolean airborne = false;
	boolean attackFinished = false;
	boolean attacking = false;

	int HP = 3;
	int visibleFrames = HP;
	int invincibilityFrames = 60;
	boolean waitFrame = false;

	int lives = 3;
	

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;				}
	wstring				getCurrentState()	{ return currentState;		}
	unsigned int		getFrameIndex()		{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType;		}
	boolean				wasJump()			{ return jump; }
	int					getHP()				{ return HP; }
	int					getVisibleFrames()  { return visibleFrames; }
	int					getInvincibilityFrames()  { return invincibilityFrames; }
	boolean				isControllable()	{ return controllable; }
	wstring				getOriginalState()  { return originalState; }
	int					getLives()			{ return lives; }
	boolean				isFacingRight()		{ return facingRight; }
	boolean				wasHit()			{ return hit; }
	boolean				isAirborne()		{ return airborne; }
	boolean				isAttackFinished()	{ return attackFinished; }
	boolean				hasWaitFrame()		{ return waitFrame; }
	boolean				isAttacking()		{ return attacking; }
	boolean				isPlayer()			{ return player; }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void setWasJump(boolean Jump)
	{
		this->jump = Jump;
	}
	void setHP(int HP)
	{
		this->HP = HP;
	}
	void decrementHP()
	{
		HP--;
	}
	void setVisibleFrames(int visibleFrames)
	{
		this->visibleFrames = visibleFrames;
	}
	void decrementVisibleFrames()
	{
		visibleFrames--;
	}
	void resetInvincibilityFrames()
	{
		invincibilityFrames = 60;
	}
	void decrementInvincibilityFrames()
	{
		invincibilityFrames--;
	}
	void toggleControllable(){
		controllable = !controllable;
	}
	void setOriginalState(wstring originalState){
		this->originalState = originalState;
	}
	void incrementLives(){
		lives++;
	}
	void decrementLives(){
		lives--;
	}
	void setLives(int lives){
		this->lives = lives;
	}
	void setFacingRight(boolean facingRight){
		this->facingRight = facingRight;
	}
	void setHit(boolean hit){
		this->hit = hit;
	}
	void setAirborne(boolean airborne){
		this->airborne = airborne;
	}
	void setAttackFinished(boolean attackFinished){
		this->attackFinished = attackFinished;
	}
	void setAttacking(boolean attacking){
		this->attacking = attacking;
	}
	void decWaitFrame(){
		waitFrame = false;
	}
	void setIsPlayer(boolean player){
		this->player = player;
	}
	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
};