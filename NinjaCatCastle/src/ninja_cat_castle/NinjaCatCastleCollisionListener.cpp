#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "ninja_cat_castle\NinjaCatCastleCollisionListener.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\ai\bots\PropellerBot.h"

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
			if (sprite1->isPlayer() && sprite2->isPlayer()){
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
				return true;
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
		if (!player->isProjectile() && !player->wasHit() && !player->isAttacking()){
			wstring enemyState = enemy->getCurrentState();
			if (enemyState != L"DIE_LEFT"  && enemyState != L"DIE_RIGHT"
				&& enemyState != L"HIT_LEFT" && enemyState != L"HIT_RIGHT"
				&& !player->getInvincibilityFrames()){
				//Marks player to send in the hit arc backwards
				player->setIsProjectile(true);

				if (!enemy->isProjectile()){
					Bot* bot = static_cast<Bot*>(enemy);
					bot->setHitPlayer(true);
				}
				else{
					enemy->markForDeletion();
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
			player->markForDeletion();
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