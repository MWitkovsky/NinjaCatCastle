#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\BombBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

BombBot::BombBot()
{
	resetThinkCycles();
}

void BombBot::throwBomb(Game *game){
	AnimatedSprite *bomb = new AnimatedSprite();
	bomb->setSpriteType(game->getGSM()->getSpriteManager()->getSpriteType(9));
	bomb->setAlpha(255);

	b2BodyDef bombProps;
	b2FixtureDef fixtureDef;
	b2PolygonShape shape;

	bombProps.position.Set(body->GetPosition().x, body->GetPosition().y);
	bombProps.type = b2_dynamicBody;
	bombProps.fixedRotation = true;

	// dimension/64/2 = float values here, NOT MAGIC NUMBERS I SWEAR
	shape.SetAsBox(0.125f, 0.125f);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 20.0f;

	bomb->setCurrentState(L"COUNTDOWN");

	if (currentState == L"THROW_BOMB_RIGHT"){
		bombProps.position.Set(body->GetPosition().x + throwOffset.x, body->GetPosition().y + throwOffset.y);
		bombProps.linearVelocity = b2Vec2(bombVelocity.x, bombVelocity.y);
		setCurrentState(L"RELOAD_RIGHT");
	}
	else{
		bombProps.position.Set(body->GetPosition().x - throwOffset.x, body->GetPosition().y + throwOffset.y);
		bombProps.linearVelocity = b2Vec2(-bombVelocity.x, bombVelocity.y);
		setCurrentState(L"RELOAD_LEFT");
	}

	bomb->setBody(game->getGSM()->getPhysics()->getWorld()->CreateBody(&bombProps));
	bomb->getBody()->CreateFixture(&fixtureDef);
	bomb->getBody()->SetUserData(bomb);
	bomb->getBody()->SetSleepingAllowed(false);
	bomb->setIsProjectile(true);

	game->getGSM()->getSpriteManager()->addProjectile(bomb);
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void BombBot::think(Game *game)
{
	if (!isDead()){
		if (!isAirborne()){
			if (cyclesRemainingBeforeThinking)
			{
				if (currentState != L"RELOAD_LEFT" && currentState != L"RELOAD_RIGHT"
					&& currentState != L"THROW_BOMB_LEFT" && currentState != L"THROW_BOMB_RIGHT"){
					b2Vec2 playerPos = game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetPosition();
					float32 diffX = body->GetPosition().x - playerPos.x;
					float32 diffY = body->GetPosition().y - playerPos.y;
					if (diffX > -seekRange.x && diffX < seekRange.x){
						if (diffY > -seekRange.y / 2.0f && diffY < seekRange.y){
							if (diffX > 0.0f){
								if (currentState != L"IDLE_LEFT"){
									setCurrentState(L"IDLE_LEFT");
								}
							}
							else{
								if (currentState != L"IDLE_RIGHT"){
									setCurrentState(L"IDLE_RIGHT");
								}
							}
						}
					}
				}
				cyclesRemainingBeforeThinking--;
			}
			else{
				b2Vec2 playerPos = game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetPosition();
				float32 diffX = body->GetPosition().x - playerPos.x;
				float32 diffY = body->GetPosition().y - playerPos.y;
				if (diffX > -seekRange.x && diffX < seekRange.x){
					if (diffY > -seekRange.y/2.0f && diffY < seekRange.y){
						if (game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetLinearVelocity().x < 0.0f){
							setCurrentState(L"THROW_BOMB_LEFT");
						}
						else if (game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetLinearVelocity().x > 0.0f){
							setCurrentState(L"THROW_BOMB_RIGHT");
						}
						else{
							if (diffX > 0.0f){
								setCurrentState(L"THROW_BOMB_LEFT");
							}
							else{
								setCurrentState(L"THROW_BOMB_RIGHT");
							}
						}
						resetThinkCycles();
					}
				}
			}
		}
	}
	animationCounter++;
}
