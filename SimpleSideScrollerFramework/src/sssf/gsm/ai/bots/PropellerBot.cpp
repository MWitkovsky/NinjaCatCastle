#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\PropellerBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
PropellerBot::PropellerBot()
{
}

void PropellerBot::shoot(Game *game){
	AnimatedSprite *projectile = new AnimatedSprite();
	projectile->setSpriteType(game->getGSM()->getSpriteManager()->getSpriteType(2));
	projectile->setCurrentState(L"THROW");
	projectile->setAlpha(255);

	b2BodyDef projectileProps;
	b2FixtureDef fixtureDef;
	b2PolygonShape shape;

	projectileProps.position.Set(body->GetPosition().x, body->GetPosition().y);
	projectileProps.type = b2_dynamicBody;
	projectileProps.fixedRotation = true;
	projectileProps.gravityScale = 0.0f;

	shape.SetAsBox(0.125f, 0.125f);
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = true;

	projectile->setBody(game->getGSM()->getPhysics()->getWorld()->CreateBody(&projectileProps));
	projectile->getBody()->CreateFixture(&fixtureDef);
	projectile->getBody()->SetUserData(projectile);
	projectile->getBody()->SetSleepingAllowed(false);

	if (isFacingRight()){
		projectile->getBody()->SetLinearVelocity(b2Vec2(projectileVelocity.x, -projectileVelocity.y));
	}
	else{
		projectile->getBody()->SetLinearVelocity(b2Vec2(-projectileVelocity.x, -projectileVelocity.y));
	}

	game->getGSM()->getSpriteManager()->addProjectile(projectile);
}

void PropellerBot::calculateBob(){
	b2Vec2 currentPosition = body->GetPosition();
	float32 currentVX = body->GetLinearVelocity().x;
	float32 currentVY = body->GetLinearVelocity().y;
	//init the movement first time
	if (currentVY == 0.0f){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}

	else if (currentPosition.y <= (originalPosition.y - bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, bobVelocity));
	}
	else if(currentPosition.y >= (originalPosition.y + bobDistance)){
		body->SetLinearVelocity(b2Vec2(currentVX, -bobVelocity));
	}
	else{
		body->SetLinearVelocity(b2Vec2(currentVX, currentVY));
	}
}

void PropellerBot::chooseFlyTrajectory(){
	b2Vec2 currentPosition = body->GetPosition();
	float32 currentVY = body->GetLinearVelocity().y;
	if (currentPosition.x <= (originalPosition.x - maxDistance)){
		if (changeDirectionWait){
			body->SetLinearVelocity(b2Vec2(0.0f, currentVY));
			changeDirectionWait--;
		}
		else{
			facingRight = true;
			body->SetLinearVelocity(b2Vec2(flyVelocity, currentVY));
			resetChangeDirectionWait();
		}
	}
	else if (currentPosition.x >= originalPosition.x){
		if (changeDirectionWait){
			body->SetLinearVelocity(b2Vec2(0.0f, currentVY));
			changeDirectionWait--;
		}
		else{
			facingRight = false;
			body->SetLinearVelocity(b2Vec2(-flyVelocity, currentVY));
			resetChangeDirectionWait();
		}
	}
	else{
		if (body->GetLinearVelocity().x <= 0.0f){
			body->SetLinearVelocity(b2Vec2(-flyVelocity, currentVY));
		}
		else{
			body->SetLinearVelocity(b2Vec2(flyVelocity, currentVY));
		}
		
	}
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void PropellerBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	if (!isDead() && !wasHit()){
		//movement logic
		if (!isAttacking()){
			chooseFlyTrajectory();
		}
		calculateBob();

		//attack logic
		if (cyclesRemainingBeforeThinking)
		{
			cyclesRemainingBeforeThinking--;
		}
		else{
			GameStateManager *gsm = game->getGSM();
			b2Vec2 playerPos = gsm->getSpriteManager()->getPlayer()->getBody()->GetPosition();
			float32 diffX = body->GetPosition().x - playerPos.x;
			float32 diffY = body->GetPosition().y - playerPos.y;
			if (facingRight){
				if (diffX > -1.0f && diffX < 3.0f){
					if (diffY > 0.0f && diffY < 12.0f){
						setCurrentState(L"JUMPING_RIGHT");
						shoot(game);
						resetThinkCycles();
					}
				}
			}
			else{
				if (diffX > -3.0f && diffX < 1.0f){
					if (diffY > 0.0f && diffY < 12.0f){
						setCurrentState(L"JUMPING_LEFT");
						shoot(game);
						resetThinkCycles();
					}
				}
			}
		}
	}
	animationCounter++;
}