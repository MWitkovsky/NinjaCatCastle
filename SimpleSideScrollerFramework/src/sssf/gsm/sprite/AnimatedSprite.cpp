/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.cpp

	See AnimatedSprite.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"

/*
	AnimatedSprite - Default constructor, just sets everything to 0.
*/
AnimatedSprite::AnimatedSprite()  
{
	spriteType = 0;
	frameIndex = 0;
	animationCounter = 0;
}

/*
	Destructor - Nothing to clean up. We don't want to clean up the
	sprite type because it is a shared variable.
*/
AnimatedSprite::~AnimatedSprite() 
{

}

/*
	changeFrame - This method allows for the changing of an image in an 
	animation sequence for a given animation state.
*/
void AnimatedSprite::changeFrame()
{
	// RESET THE COUNTER
	animationCounter = 0;

	// REMEMBER, WE COUNT BY 2s
	frameIndex += 2;

	// GO BACK TO FIRST INDEX IF NECESSARY
	// THIS IS ALSO WHERE ALL ANIMATIONS RELIANT ON OTHER ANIMATIONS ARE HANDLED

	if (frameIndex == spriteType->getSequenceSize(currentState)){
		if (!wasHit() && !isDead()){
			//THIS IS ALL PLAYER ANIMATION STUFF THAT IS RELIANT
			//ON THE END OF AN ANIMATION (Example: When attack ends, set to idle state)
			if (currentState == L"JUMPING_ARC_RIGHT"){
				setCurrentState(L"JUMPING_DESCEND_RIGHT");
			}
			else if (currentState == L"JUMPING_ARC_LEFT"){
				setCurrentState(L"JUMPING_DESCEND_LEFT");
			}
			else if (attacking){
				attackFinished = true;
				waitFrame = true;
				float32 vY = body->GetLinearVelocity().y;
				float32 vX = body->GetLinearVelocity().x;
				if (vY == 0){
					if (airborneGuard){
						airborneGuard = false;
					}
					else{
						setAirborne(false);
						if (vX > 0){
							setCurrentState(L"WALK_RIGHT");
						}
						else if (vX < 0){
							setCurrentState(L"WALK_LEFT");
						}
						else{
							if (currentState == L"ATTACK_RIGHT" || currentState == L"ATTACK_RIGHT_2"){
								setCurrentState(L"IDLE_RIGHT");
							}
							else{
								setCurrentState(L"IDLE_LEFT");
							}
						}
					}
				}
				else if(vY < 0){
					if (vX > 0){
						setCurrentState(L"JUMPING_DESCEND_RIGHT");
					}
					else if (vX < 0){
						setCurrentState(L"JUMPING_DESCEND_LEFT");
					}
					else{
						if (currentState == L"ATTACK_RIGHT" || currentState == L"ATTACK_RIGHT_2"){
							setCurrentState(L"JUMPING_DESCEND_RIGHT");
						}
						else{
							setCurrentState(L"JUMPING_DESCEND_LEFT");
						}
					}
				}
				else if (vY > 0){
					if (vX > 0){
						setCurrentState(L"JUMPING_ASCEND_RIGHT");
					}
					else if (vX < 0){
						setCurrentState(L"JUMPING_ASCEND_LEFT");
					}
					else{
						if (currentState == L"ATTACK_RIGHT" || currentState == L"ATTACK_RIGHT_2"){
							setCurrentState(L"JUMPING_ASCEND_RIGHT");
						}
						else{
							setCurrentState(L"JUMPING_ASCEND_LEFT");
						}
					}
				}
				else{
					if (vX > 0){
						setCurrentState(L"JUMPING_ARC_RIGHT");
					}
					else if (vX < 0){
						setCurrentState(L"JUMPING_ARC_LEFT");
					}
					else{
						if (currentState == L"ATTACK_RIGHT" || currentState == L"ATTACK_RIGHT_2"){
							setCurrentState(L"JUMPING_ARC_RIGHT");
						}
						else{
							setCurrentState(L"JUMPING_ARC_LEFT");
						}
					}
				}
			}
			else if (currentState == L"LAND_LEFT" || currentState == L"LAND_RIGHT"){
				if (currentState == L"LAND_LEFT"){
					setCurrentState(L"IDLE_LEFT");
				}
				else{
					setCurrentState(L"IDLE_RIGHT");
				}
			}
			else if (currentState == L"JUMP_START_LEFT"){
				setCurrentState(L"JUMPING_LEFT");
			}
			else if (currentState == L"JUMP_START_RIGHT"){
				setCurrentState(L"JUMPING_RIGHT");
			}
			else{
				frameIndex = 0;
			}
		}
		else{
			frameIndex = frameIndex -= 2;
		}
	}
}

/*
	getCurrentImageID - This method does the work of retrieving
	the image id that corresponds to this sprite's current state
	and frame index.
*/
unsigned int AnimatedSprite::getCurrentImageID()
{
	return spriteType->getAnimationFrameID(currentState, frameIndex);
}

/*
	setCurrentState - This method sets this sprite to the newState
	state and starts its animtion sequence from the beginning.
*/
void AnimatedSprite::setCurrentState(wstring newState) 
{
	string cs(currentState.begin(), currentState.end());
	string ns(newState.begin(), newState.end());
	if (strcmp(cs.c_str(), ns.c_str()) != 0)
	{
		// SET THE ANIMATINO STATE
		currentState = newState;

		// AND RESET OUR COUNTERS
		animationCounter = 0;
		frameIndex = 0;
	}
}

/*
	updateSprite - To be called every frame of animation. This
    method advances the animation counter appropriately per
	the animation speed. It also updates the sprite location
	per the current velocity.
*/
void AnimatedSprite::updateSprite()
{
	unsigned int duration = spriteType->getDuration(currentState, frameIndex);
	animationCounter++;

	// WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
	// ANIMATION COUNTER HAS REACHED THE DURATION
	if (animationCounter >= duration)
		changeFrame();
}