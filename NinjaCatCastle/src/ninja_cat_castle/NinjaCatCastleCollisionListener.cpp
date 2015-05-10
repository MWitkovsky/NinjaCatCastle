#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "ninja_cat_castle\NinjaCatCastleCollisionListener.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\ai\bots\PropellerBot.h"
#include "sssf\gsm\ai\bots\BombBot.h"
#include "sssf\gsm\ai\bots\ArmorBot.h"
#include "sssf\gsm\ai\bots\Pickup.h"

void NinjaCatCastleCollisionListener::BeginContact(b2Contact* contact) {

	//Checks if either of the two fixtures belong to the player.
	AnimatedSprite* sprite1 = 0;
	AnimatedSprite* sprite2 = 0;

	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData){
		sprite1 = static_cast<AnimatedSprite*>(bodyUserData);
	}

	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData){
		sprite2 = static_cast<AnimatedSprite*>(bodyUserData);
	}

	//Fixes the two sprites up nicely so the player is always first,
	//then calls the helper method respondToCollision
	if (sprite1 && sprite2){
		if (sprite1->isPlayer() && sprite2->isPlayer()){
			return;
		}
		else if (sprite1->isPlayer()){
			respondToCollision(sprite1, sprite2, contact);
		}
		else if (sprite2->isPlayer()){
			respondToCollision(sprite2, sprite1, contact);
		}
	}
}

bool NinjaCatCastleCollisionListener::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB){
	//Static bodies always interact no matter what (for now)
	if (fixtureA->GetType() == b2_staticBody || fixtureB->GetType() == b2_staticBody){
		return true;
	}

	AnimatedSprite* sprite1 = 0;
	AnimatedSprite* sprite2 = 0;

	void* bodyUserData = fixtureA->GetBody()->GetUserData();
	if (bodyUserData){
		sprite1 = static_cast<AnimatedSprite*>(bodyUserData);
	}

	bodyUserData = fixtureB->GetBody()->GetUserData();
	if (bodyUserData){
		sprite2 = static_cast<AnimatedSprite*>(bodyUserData);
	}

	if (sprite1 && sprite2){
		if (sprite1->isPlayer() || sprite2->isPlayer()){

			if (sprite1->isPlayer()){
				sprite2->setTrajectory(sprite2->getBody()->GetLinearVelocity());
			}
			else if (sprite2->isPlayer()){
				sprite1->setTrajectory(sprite1->getBody()->GetLinearVelocity());
			}

			if (sprite1->isPlayer() && sprite2->isPlayer()){
				return false;
			}
			else if (sprite1->isProjectile() && sprite2->isProjectile()){
				return false;
			}
			else if (sprite1->wasHit() || sprite2->wasHit() || sprite1->isDead() || sprite2->isDead()){
				return false;
			}
			else if ((sprite1->isProjectile() && fixtureB->IsSensor())
				|| (sprite2->isProjectile() && fixtureA->IsSensor())){
				return false;
			}
			else if (fixtureA->IsSensor() || fixtureB->IsSensor()){
				wstring enemyState = sprite1->getCurrentState();
				if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"
					&& enemyState != L"HIT_LEFT" && enemyState != L"HIT_RIGHT"){
					return true;
				}
				else{
					enemyState = sprite2->getCurrentState();
					if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"
						&& enemyState != L"HIT_LEFT" && enemyState != L"HIT_RIGHT"){
						return true;
					}
				}
				return false;
			}
			else if (sprite1->getInvincibilityFrames() || sprite2->getInvincibilityFrames()){
				return false;
			}
			else{
				return true;
			}
		}
		else{
			return false;
		}
	}
	else{
		return true;
	}
}

void NinjaCatCastleCollisionListener::respondToCollision(AnimatedSprite *player, AnimatedSprite *enemy, b2Contact* contact)
{
	b2Fixture *playerBox = player->getBody()->GetFixtureList();
	if (contact->GetFixtureA() == playerBox || contact->GetFixtureB() == playerBox){
		Pickup* pickup = dynamic_cast<Pickup*>(enemy);
		if (pickup && !player->isProjectile()){
			if (pickup->getAlpha() != 0){
				pickup->setAlpha(0);
				if (pickup->isHealth()){
					if (player->getHP() < 3){
						player->setHP(player->getHP() + 1);
						player->setShouldPlayHealthSound(true);
					}
					else{
						pickup->setAlpha(255);
					}
				}
				else{
					player->collectTreat();
					player->setShouldPlayTreatSound(true);
				}
			}
			return;
		}
		if (!player->isProjectile() && !player->wasHit()){
			wstring enemyState = enemy->getCurrentState();
			if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"
				&& enemyState != L"HIT_LEFT" && enemyState != L"HIT_RIGHT"
				&& !player->getInvincibilityFrames()){
				ArmorBot* armorBot = dynamic_cast<ArmorBot*>(enemy);
				if (armorBot){
					if (contact->GetFixtureA() == armorBot->getAttackBox() || contact->GetFixtureB() == armorBot->getAttackBox()){
						if (enemyState == L"ATTACK_LEFT_A" || enemyState == L"ATTACK_RIGHT_A"){
							//Marks player to send in the hit arc backwards
							player->setIsProjectile(true);

							if (!enemy->isProjectile()){
								Bot* bot = static_cast<Bot*>(enemy);
								bot->setHitPlayer(true);
							}
							else{
								enemy->markForDeletion(true);
							}

							if (player->getHurtBox()){
								player->setAttackFinished(true);
								player->setAttacking(false);
							}
							if (player->isFacingRight()){
								player->setCurrentState(L"HIT_LEFT");
							}
							else{
								player->setCurrentState(L"HIT_RIGHT");
							}
							player->decrementHP();
							player->setHit(true);
						}
					}
					else{
						//Marks player to send in the hit arc backwards
						player->setIsProjectile(true);

						if (!enemy->isProjectile()){
							Bot* bot = static_cast<Bot*>(enemy);
							bot->setHitPlayer(true);
						}
						else{
							enemy->markForDeletion(true);
						}

						if (player->getHurtBox()){
							player->setAttackFinished(true);
							player->setAttacking(false);
						}
						if (player->isFacingRight()){
							player->setCurrentState(L"HIT_LEFT");
						}
						else{
							player->setCurrentState(L"HIT_RIGHT");
						}
						player->decrementHP();
						player->setHit(true);
					}
				}
				else{
					//Marks player to send in the hit arc backwards
					player->setIsProjectile(true);

					if (!enemy->isProjectile()){
						Bot* bot = static_cast<Bot*>(enemy);
						bot->setHitPlayer(true);
					}
					else{
						enemy->markForDeletion(true);
					}

					if (player->getHurtBox()){
						player->setAttackFinished(true);
						player->setAttacking(false);
					}
					if (player->isFacingRight()){
						player->setCurrentState(L"HIT_LEFT");
					}
					else{
						player->setCurrentState(L"HIT_RIGHT");
					}
					player->decrementHP();
					player->setHit(true);
				}	
			}
		}
		else{
			if (player->isAttacking() || player->getCurrentState() == L"THROW"){
				if (player->getCurrentState() == L"THROW"){
					player->markForDeletion(true);
				}
				wstring enemyState = enemy->getCurrentState();
				ArmorBot* armorBot = dynamic_cast<ArmorBot*>(enemy);
				if (armorBot){
					if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"){
						if (contact->GetFixtureA() != armorBot->getAttackBox() && contact->GetFixtureB() != armorBot->getAttackBox()){
							if (armorBot->isVulnerable()){
								enemy->markForDeletion(true);
								if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
									enemy->setCurrentState(L"DIE_RIGHT");
								}
								else{
									enemy->setCurrentState(L"DIE_LEFT");
								}
								enemy->setHit(true);
								enemy->setPlayHitSound(true);
								enemy->getBody()->SetGravityScale(1.0f);
							}
							else{
								armorBot->resetThinkCyclesBlock();
								if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
									enemy->setCurrentState(L"BLOCK_RIGHT");
									enemy->getBody()->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));
								}
								else{
									enemy->setCurrentState(L"BLOCK_LEFT");
									enemy->getBody()->SetLinearVelocity(b2Vec2(5.0f, 0.0f));
								}
							}
						}
					}
				}
				else if (dynamic_cast<BombBot*>(enemy)){
					if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"){
						enemy->markForDeletion(true);
						if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
							enemy->setCurrentState(L"DIE_RIGHT");
						}
						else{
							enemy->setCurrentState(L"DIE_LEFT");
						}
						enemy->setHit(true);
						enemy->setPlayHitSound(true);
						enemy->getBody()->SetGravityScale(1.0f);
					}
				}
				else{
					if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"
						&& enemyState != L"HIT_LEFT" && enemyState != L"HIT_RIGHT"){
						enemy->markForDeletion(true);
						if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
							enemy->setCurrentState(L"HIT_RIGHT");
							enemy->getBody()->SetLinearVelocity(b2Vec2(-3.0f, 8.0f));
						}
						else{
							enemy->setCurrentState(L"HIT_LEFT");
							enemy->getBody()->SetLinearVelocity(b2Vec2(3.0f, 8.0f));
						}
						enemy->setHit(true);
						enemy->setPlayHitSound(true);
						enemy->getBody()->SetGravityScale(1.0f);
					}
				}
			}
			
		}
	}
	else{
		if (!dynamic_cast<Pickup*>(enemy)){
			ArmorBot* armorBot = dynamic_cast<ArmorBot*>(enemy);
			wstring enemyState = enemy->getCurrentState();
			if (armorBot){
				if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"){
					if (contact->GetFixtureA() != armorBot->getAttackBox() && contact->GetFixtureB() != armorBot->getAttackBox()){
						if (armorBot->isVulnerable()){
							enemy->markForDeletion(true);
							if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
								enemy->setCurrentState(L"DIE_RIGHT");
							}
							else{
								enemy->setCurrentState(L"DIE_LEFT");
							}
							enemy->setHit(true);
							enemy->setPlayHitSound(true);
							enemy->getBody()->SetGravityScale(1.0f);
						}
						else{
							armorBot->resetThinkCyclesBlock();
							if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
								enemy->setCurrentState(L"BLOCK_RIGHT");
								enemy->getBody()->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));
							}
							else{
								enemy->setCurrentState(L"BLOCK_LEFT");
								enemy->getBody()->SetLinearVelocity(b2Vec2(5.0f, 0.0f));
							}
						}
					}
				}
			}
			else if (dynamic_cast<BombBot*>(enemy)){
				if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"){
					enemy->markForDeletion(true);
					if (enemy->getBody()->GetPosition().x < player->getBody()->GetPosition().x){
						enemy->setCurrentState(L"DIE_RIGHT");
					}
					else{
						enemy->setCurrentState(L"DIE_LEFT");
					}
					enemy->setHit(true);
					enemy->setPlayHitSound(true);
					enemy->getBody()->SetGravityScale(1.0f);
				}
			}
			else{
				if (enemy->getBody()->GetPosition().x < player->getHurtBox()->GetPosition().x){
					enemy->setCurrentState(L"HIT_RIGHT");
					if (player->getBody()->GetLinearVelocity().x < 0.0f){
						enemy->getBody()->SetLinearVelocity(b2Vec2(-8.0f, 8.0f));
					}
					else{
						enemy->getBody()->SetLinearVelocity(b2Vec2(-3.0f, 8.0f));
					}
				}
				else{
					enemy->setCurrentState(L"HIT_LEFT");
					if (player->getBody()->GetLinearVelocity().x > 0.0f){
						enemy->getBody()->SetLinearVelocity(b2Vec2(8.0f, 8.0f));
					}
					else{
						enemy->getBody()->SetLinearVelocity(b2Vec2(3.0f, 8.0f));
					}
				}
				enemy->setHit(true);
				enemy->setPlayHitSound(true);
				enemy->getBody()->SetGravityScale(1.0f);
			}
		}
	}
}