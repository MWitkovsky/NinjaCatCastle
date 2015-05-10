#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\ArmorBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

ArmorBot::ArmorBot()
{
	resetThinkCycles();
}

boolean ArmorBot::walk(Game *game, float32 diffX, float32 diffY){
	if (!isBotAttacking() && !isBlocking()){
		if (diffY <= approachRange.y && diffY >= -approachRange.y){
			if (diffX >= -approachRange.x && diffX < 0.0f){
				if (currentState != L"WALK_RIGHT"){
					setCurrentState(L"WALK_RIGHT");
				}
				body->SetLinearVelocity(b2Vec2(walkVelocity, 0.0f));
				return true;
			}
			else if (diffX <= approachRange.x && diffX > 0.0f){
				if (currentState != L"WALK_LEFT"){
					setCurrentState(L"WALK_LEFT");
				}
				body->SetLinearVelocity(b2Vec2(-walkVelocity, 0.0f));
				return true;
			}
		}
		return false;
	}
	return false;
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void ArmorBot::think(Game *game)
{
	if (!isDead()){
		if (!isAirborne()){
			b2Vec2 playerPos = game->getGSM()->getSpriteManager()->getPlayer()->getBody()->GetPosition();
			float32 diffX = body->GetPosition().x - playerPos.x;
			float32 diffY = body->GetPosition().y - playerPos.y;

			if (cyclesRemainingBeforeThinking){
				if (!walk(game, diffX, diffY)){
					if (!isBlocking() && !isBotAttacking()){
						if (diffX > 0){
							setCurrentState(L"IDLE_LEFT");
						}
						else{
							setCurrentState(L"IDLE_RIGHT");
						}
					}
				}
				cyclesRemainingBeforeThinking--;
			}
			else{
				GameStateManager *gsm = game->getGSM();
				if (diffY <= attackRange.y && diffY >= -attackRange.y){
					if (diffX >= -attackRange.x && diffX < 0.0f){
						setCurrentState(L"WINDUP_RIGHT");
						resetThinkCycles();
					}
					else if (diffX <= attackRange.x && diffX > 0.0f){
						setCurrentState(L"WINDUP_LEFT");
						resetThinkCycles();
					}
				}
				
				if (!walk(game, diffX, diffY)){
					if (!isBotAttacking() && !isBlocking()){
						if (diffX > 0){
							setCurrentState(L"IDLE_LEFT");
						}
						else{
							setCurrentState(L"IDLE_RIGHT");
						}
					}
				}

			}
		}
	}
	animationCounter++;
}
	