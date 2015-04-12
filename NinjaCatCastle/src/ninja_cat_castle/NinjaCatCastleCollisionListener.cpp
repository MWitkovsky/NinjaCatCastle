#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "ninja_cat_castle\NinjaCatCastleCollisionListener.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\ai\bots\RandomFloatingBot.h"

void NinjaCatCastleCollisionListener::respondToCollision(Collision *collision)
{
	// NOTE FROM THE COLLIDABLE OBJECTS, WHICH ARE IN THE COLLISION,
	// WE CAN CHECK AND SEE ON WHICH SIDE THE COLLISION HAPPENED AND
	// CHANGE SOME APPROPRIATE STATE ACCORDINGLY

	/*if (!collision->isCollisionWithTile())
	{
		CollidableObject *sprite = collision->getCO1();
		AnimatedSprite* player = dynamic_cast<AnimatedSprite*>(sprite);
		if (player->getCurrentState() != DIE){
			CollidableObject *sprite2 = collision->getCO2();
			AnimatedSprite* enemy = dynamic_cast<AnimatedSprite*>(collision->getCO2());
			RandomJumpingBot *jumpingBotCheck = dynamic_cast<RandomJumpingBot*>(enemy);
			RandomFloatingBot *floatingBotCheck = dynamic_cast<RandomFloatingBot*>(enemy);

			if (jumpingBotCheck){
				if (enemy->getCurrentState() != DIE){
					if (jumpingBotCheck->getSpriteType()->getSpriteTypeID() == 1){
						if (sprite2->getBoundingVolume()->getCenterY() > sprite->getBoundingVolume()->getBottom()){
							// ENEMY IS DEAD - WE SHOULD PLAY A DEATH ANIMATION
							// AND MARK IT FOR REMOVAL
							if (enemy->getInvincibilityFrames() == 0){
								if (player->getPhysicalProperties()->getVelocityY() > 0)
									player->getPhysicalProperties()->setVelocity(player->getPhysicalProperties()->getVelocityX(), -player->getPhysicalProperties()->getVelocityY());

								enemy->getPhysicalProperties()->setVelocity(0, 0);
								enemy->setCurrentState(DIE);
							}
						}
						else{
							if (player->getInvincibilityFrames() == 0){
								if (enemy->getPhysicalProperties()->getVelocityY() > 0)
									enemy->getPhysicalProperties()->setVelocity(enemy->getPhysicalProperties()->getVelocityX(), -enemy->getPhysicalProperties()->getVelocityY());

								if (player->getHP() != 1){
									player->decrementHP();
									player->resetInvincibilityFrames();
								}
								else{
									// PLAYER IS DEAD - WE SHOULD PLAY A DEATH ANIMATION
									// AND MARK IT FOR REMOVAL/RESPAWN/RESTART GAME, WHATEVER
									// THE DEMANDS OF THE GAME ARE
									player->decrementHP();
									player->getPhysicalProperties()->setVelocity(0, player->getPhysicalProperties()->getVelocityY());
									player->setCurrentState(DIE);
									player->toggleControllable();
									player->setVisibleFrames(100);
								}
							}
						}
					}
					else{
						if (player->getInvincibilityFrames() == 0){
							if (player->getHP() != 10){
								player->setHP(player->getHP() + 1);
								player->resetInvincibilityFrames();
							}
						}
					}
				}
			}
			else if (floatingBotCheck){
				if (player->getInvincibilityFrames() == 0){
					while (player->getHP() != 0){
						player->decrementHP();
					}
					// PLAYER IS DEAD - WE SHOULD PLAY A DEATH ANIMATION
					// AND MARK IT FOR REMOVAL/RESPAWN/RESTART GAME, WHATEVER
					// THE DEMANDS OF THE GAME ARE

					player->getPhysicalProperties()->setVelocity(0, player->getPhysicalProperties()->getVelocityY());
					player->setCurrentState(DIE);
					player->toggleControllable();
					player->setVisibleFrames(100);
				}
			}
		}
	}*/
}