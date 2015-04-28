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
#include "sssf\game\Game.h"
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
	boolean projectile = false;
	boolean controllable = true;
	boolean jump = false;
	boolean facingRight = false;
	boolean hit = false;
	boolean airborne = false;
	boolean airborneGuard = false;
	boolean attackFinished = false;
	boolean attacking = false;
	boolean toBeDeleted = false;
	boolean playHitSound = false;

	int HP = 3;
	int visibleFrames = HP;
	int invincibilityFrames = 0;
	boolean waitFrame = false;

	int lives = 3;
	int treats = 0;
	

public:
	// INLINED ACCESSOR METHODS
	unsigned int		getFrameIndex()		{ return frameIndex; }
	int					getAlpha()			{ return alpha;				}
	int					getHP()				{ return HP; }
	int					getVisibleFrames()  { return visibleFrames; }
	int					getInvincibilityFrames()  { return invincibilityFrames; }
	int					getLives()			{ return lives; }
	int					getTreats()			{ return treats; }
	boolean				wasJump()			{ return jump; }
	boolean				isControllable()	{ return controllable; }
	boolean				isFacingRight()		{ return facingRight; }
	boolean				wasHit()			{ return hit; }
	boolean				isAirborne()		{ return airborne; }
	boolean				isAttackFinished()	{ return attackFinished; }
	boolean				hasWaitFrame()		{ return waitFrame; }
	boolean				isAttacking()		{ return attacking; }
	boolean				isPlayer()			{ return player; }
	boolean				isDead()			{ return (currentState == L"DIE_LEFT" || currentState == L"DIE_RIGHT"); }
	boolean				isProjectile()		{ return projectile; }
	boolean				isMarkedForDeletion()	{ return toBeDeleted; }
	boolean				shouldPlayHitSound()	{ return playHitSound; }
	wstring				getCurrentState()	{ return currentState; }
	wstring				getOriginalState()  { return originalState; }
	AnimatedSpriteType*	getSpriteType()		{ return spriteType; }

	//"Airborne Guard" is for making sure animations don't break as currently they're handled by
	//checking if velocity is zero. In any given arc, the velocity is zero twice, once at the peak
	//and once when it lands on the ground. This makes it so things don't change when at the peak
	//of an arc unintentionally.
	boolean				hasAirborneGuard()  { return airborneGuard; }

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
		invincibilityFrames = 120;
	}
	void decrementInvincibilityFrames()
	{
		invincibilityFrames--;
	}
	void setControllable(boolean controllable){
		this->controllable = controllable;
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
		if (hit){
			resetInvincibilityFrames();
			airborneGuard = true;
			controllable = false;
		}
		else{
			controllable = true;
		}
	}
	void setAirborne(boolean airborne){
		this->airborne = airborne;
		airborneGuard = airborne;
	}
	void setAttackFinished(boolean attackFinished){
		this->attackFinished = attackFinished;
	}
	void setAttacking(boolean attacking){
		this->attacking = attacking;
		attackFinished = !attacking;
	}
	void decWaitFrame(){
		waitFrame = false;
	}
	void setIsPlayer(boolean player){
		this->player = player;
	}
	void setAirborneGuard(boolean airborneGuard){
		this->airborneGuard = airborneGuard;
	}
	void setIsProjectile(boolean projectile){
		this->projectile = projectile;
	}
	void markForDeletion(){
		toBeDeleted = !toBeDeleted;
	}
	void setPlayHitSound(boolean playHitSound){
		this->playHitSound = playHitSound;
	}
	//For testing!
	void setInvincibilityFrames(int invincibilityFrames){
		this->invincibilityFrames = invincibilityFrames;
	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame(Game *game);
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite(Game *game);
	void collectTreat();
};