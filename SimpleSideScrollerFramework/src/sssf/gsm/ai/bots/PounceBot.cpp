#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

PounceBot::PounceBot()
{
	resetThinkCycles();
}

void PounceBot::Jump(boolean jumpRight)
{
	b2PolygonShape boxShape;
	b2FixtureDef fixtureDef;
	b2FixtureDef boxFixtureDef;
	if (body->GetFixtureList()){
		body->DestroyFixture(body->GetFixtureList());
	}

	if (jumpRight){
		boxShape.SetAsBox(0.7f, 0.4f, b2Vec2(0.0f, 0.0f), 0.5f);
		body->SetLinearVelocity(b2Vec2(jumpVelocityX, jumpVelocityY));
	}
	else{
		boxShape.SetAsBox(0.7f, 0.4f, b2Vec2(0.0f, 0.0f), 2.6f);
		body->SetLinearVelocity(b2Vec2(-jumpVelocityX, jumpVelocityY));
	}

	boxFixtureDef.shape = &boxShape;
	body->CreateFixture(&boxFixtureDef);
	setAirborne(true);
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void PounceBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	if (!isDead()){
		//POUNCING LOGIC GOES HERE
		if (!isAirborne()){
			if (cyclesRemainingBeforeThinking){
				cyclesRemainingBeforeThinking--;
				if (currentState == L"JUMPING_LEFT"){
					setCurrentState(L"IDLE_LEFT");

					if (body->GetFixtureList()){
						body->DestroyFixture(body->GetFixtureList());
					}
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;
					shape.SetAsBox(0.7f, 0.4f);
					fixtureDef.shape = &shape;
					body->CreateFixture(&fixtureDef);
					body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
				else if (currentState == L"JUMPING_RIGHT"){
					setCurrentState(L"IDLE_RIGHT");

					if (body->GetFixtureList()){
						body->DestroyFixture(body->GetFixtureList());
					}
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;
					shape.SetAsBox(0.7f, 0.4f);
					fixtureDef.shape = &shape;
					body->CreateFixture(&fixtureDef);
					body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
			}
			else{
				GameStateManager *gsm = game->getGSM();
				b2Vec2 playerPos = gsm->getSpriteManager()->getPlayer()->getBody()->GetPosition();
				float32 diffX = body->GetPosition().x - playerPos.x;
				float32 diffY = body->GetPosition().y - playerPos.y;
				if (diffX > -9.0f && diffX < 9.0f){
					if (diffY > -2.0f && diffY < 0.0f){
						if (diffX > 0){
							setCurrentState(L"JUMP_START_LEFT");
						}
						else{
							setCurrentState(L"JUMP_START_RIGHT");
						}
						resetThinkCycles();
					}
				}
			}
		}
	}
	animationCounter++;
}