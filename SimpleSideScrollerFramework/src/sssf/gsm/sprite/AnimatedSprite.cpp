/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.cpp

	See AnimatedSprite.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\ai\bots\PropellerBot.h"
#include "sssf\gsm\ai\bots\BombBot.h"

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
void AnimatedSprite::changeFrame(Game *game)
{
	// RESET THE COUNTER
	animationCounter = 0;

	// REMEMBER, WE COUNT BY 2s
	frameIndex += 2;

	// GO BACK TO FIRST INDEX IF NECESSARY
	// THIS IS ALSO WHERE ALL ANIMATIONS RELIANT ON OTHER ANIMATIONS ARE HANDLED

	if (frameIndex == spriteType->getSequenceSize(currentState)){
		if (!wasHit() && !isDead()){
			//THIS IS ALL ANIMATION STUFF THAT IS RELIANT
			//ON THE END OF AN ANIMATION (Example: When attack ends, set to idle state)
			if (currentState == L"JUMPING_ARC_RIGHT"){
				if (body->GetFixtureList()){
					body->DestroyFixture(body->GetFixtureList());
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
				body->CreateFixture(&fixtureDef);

				setCurrentState(L"JUMPING_DESCEND_RIGHT");

			}
			else if (currentState == L"JUMPING_ARC_LEFT"){
				if (body->GetFixtureList()){
					body->DestroyFixture(body->GetFixtureList());
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
				body->CreateFixture(&fixtureDef);

				setCurrentState(L"JUMPING_DESCEND_LEFT");
			}
			else if (currentState == L"THROW_LEFT" || currentState == L"THROW_RIGHT"){
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
							if (currentState == L"THROW_RIGHT"){
								setCurrentState(L"IDLE_RIGHT");
							}
							else{
								setCurrentState(L"IDLE_LEFT");
							}
						}
					}
				}
				else if (vY < 0){
					if (vX > 0){
						setCurrentState(L"JUMPING_DESCEND_RIGHT");
					}
					else if (vX < 0){
						setCurrentState(L"JUMPING_DESCEND_LEFT");
					}
					else{
						if (currentState == L"THROW_RIGHT"){
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
						if (currentState == L"THROW_RIGHT"){
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
						if (currentState == L"THROW_RIGHT"){
							setCurrentState(L"JUMPING_ARC_RIGHT");
						}
						else{
							setCurrentState(L"JUMPING_ARC_LEFT");
						}
					}
				}
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
				game->playSound(SOUND_CAT_POUNCE);
				setCurrentState(L"JUMPING_LEFT");
			}
			else if (currentState == L"JUMP_START_RIGHT"){
				game->playSound(SOUND_CAT_POUNCE);
				setCurrentState(L"JUMPING_RIGHT");
			}
			else if (currentState == L"SHOOT_LEFT" || currentState == L"SHOOT_RIGHT"){
				static_cast<PropellerBot*>(this)->shoot(game);
			}
			else if (currentState == L"THROW_BOMB_LEFT" || currentState == L"THROW_BOMB_RIGHT"){
				static_cast<BombBot*>(this)->throwBomb(game);
			}
			else if (currentState == L"RELOAD_LEFT" || currentState == L"RELOAD_RIGHT"){
				if (currentState == L"RELOAD_LEFT"){
					setCurrentState(L"IDLE_LEFT");
				}
				else{
					setCurrentState(L"IDLE_RIGHT");
				}
			}
			else if (currentState == L"COUNTDOWN"){
				setCurrentState(L"EXPLODE");
				if (body->GetFixtureList()){
					body->DestroyFixture(body->GetFixtureList());
				}
				b2FixtureDef fixtureDef;
				b2PolygonShape shape;
				shape.SetAsBox(0.4f, 0.4f);
				fixtureDef.shape = &shape;
				body->CreateFixture(&fixtureDef);

				game->playSound(SOUND_BOMB_EXPLOSION1);
				game->playSound(SOUND_BOMB_EXPLOSION2);
			}
			else if (currentState == L"EXPLODE"){
				setAlpha(0);
				setCurrentState(L"EXPLODE2");
				if (body){
					game->getGSM()->getPhysics()->getWorld()->DestroyBody(body);
					body = NULL;
				}
			}
			else if (currentState == L"WINDUP_LEFT"){
				setCurrentState(L"ATTACK_LEFT_A");
				game->playSound(SOUND_BIGSLASH);
			}
			else if (currentState == L"WINDUP_RIGHT"){
				setCurrentState(L"ATTACK_RIGHT_A");
				game->playSound(SOUND_BIGSLASH);
			}
			else if (currentState == L"ATTACK_LEFT_A"){
				setCurrentState(L"ATTACK_RECOVER_LEFT");
			}
			else if (currentState == L"ATTACK_RIGHT_A"){
				setCurrentState(L"ATTACK_RECOVER_RIGHT");
			}
			else if (currentState == L"ATTACK_RECOVER_LEFT"){
				setCurrentState(L"IDLE_LEFT");
			}
			else if (currentState == L"ATTACK_RECOVER_RIGHT"){
				setCurrentState(L"IDLE_RIGHT");
			}
			else if (currentState == L"BLOCK_LEFT"){
				setCurrentState(L"IDLE_LEFT");
			}
			else if (currentState == L"BLOCK_RIGHT"){
				setCurrentState(L"IDLE_RIGHT");
			}
			else{
				frameIndex = 0;
			}
		}
		else if (dynamic_cast<PropellerBot*>(this)){
			if (currentState == L"HIT_LEFT" || currentState == L"HIT_RIGHT"){
				frameIndex = 0;
			}
			else{
				frameIndex -= 2;
			}
		}
		else if (isPlayer() && isDead()){
			decrementLives();
			if (lives != 0){
				game->quitGame();
				game->startGame();
			}
			else{
				game->quitGame();
				introChannel = game->playSongIntro(MAIN_MENU_SONG_INTRO, introChannel);
				musicChannel = game->queueSong(MAIN_MENU_SONG, musicChannel, 190);
			}
		}
		else{
			frameIndex -= 2;
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
void AnimatedSprite::updateSprite(Game *game)
{
	unsigned int duration = spriteType->getDuration(currentState, frameIndex);
	animationCounter++;

	// WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
	// ANIMATION COUNTER HAS REACHED THE DURATION
	if (animationCounter >= duration)
		changeFrame(game);
}

void AnimatedSprite::collectTreat(){
	treats++;
}