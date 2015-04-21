/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaCatCastleKeyEventHandler.cpp

	See NinjaCatCastleKeyEventHandler.h for a class description.
*/

#include "ninja_cat_castle_VS\stdafx.h"
#include "ninja_cat_castle\NinjaCatCastle.h"
#include "ninja_cat_castle\NinjaCatCastleKeyEventHandler.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"
#include "Box2D\Box2D.h"

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void NinjaCatCastleKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	b2Body *pp = player->getBody();
	Viewport *viewport = game->getGUI()->getViewport();
	
	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->GetLinearVelocity().x;
		float vY = pp->GetLinearVelocity().y;
		wstring state = player->getCurrentState();

		if (player->isControllable()){
			if (!(input->isKeyDown(A_KEY) && input->isKeyDown(D_KEY))){
				if (input->isKeyDown(A_KEY))
				{
					if (!player->isAirborne()){
						if (state != L"ATTACK_LEFT" && state != L"ATTACK_RIGHT"
							&& state != L"ATTACK_LEFT_2" && state != L"ATTACK_RIGHT_2"){
							player->setCurrentState(L"WALK_LEFT");
							player->setFacingRight(false);
						}
						vX = -PLAYER_SPEED;
					}

					//Just changes the way the sprite is facing, doesn't change anything about the jump.
					if (state == L"JUMPING_DESCEND_RIGHT"){
						player->setCurrentState(L"JUMPING_DESCEND_LEFT");
						player->setFacingRight(false);
					}
					else if (state == L"JUMPING_ARC_RIGHT"){
						player->setCurrentState(L"JUMPING_ARC_LEFT");
						player->setFacingRight(false);
					}
					else if (state == L"JUMPING_ASCEND_RIGHT"){
						player->setCurrentState(L"JUMPING_ASCEND_LEFT");
						player->setFacingRight(false);
					}
				}
				else if (input->isKeyDown(D_KEY))
				{
					if (!player->isAirborne()){
						if (state != L"ATTACK_LEFT" && state != L"ATTACK_RIGHT"
							&& state != L"ATTACK_LEFT_2" && state != L"ATTACK_RIGHT_2"){
							player->setCurrentState(L"WALK_RIGHT");
							player->setFacingRight(true);
						}
						vX = PLAYER_SPEED;
					}

					//Just changes the way the sprite is facing, doesn't change anything about the jump.
					if (state == L"JUMPING_DESCEND_LEFT"){
						player->setCurrentState(L"JUMPING_DESCEND_RIGHT");
						player->setFacingRight(true);
					}
					else if (state == L"JUMPING_ARC_LEFT"){
						player->setCurrentState(L"JUMPING_ARC_RIGHT");
						player->setFacingRight(true);
					}
					else if (state == L"JUMPING_ASCEND_LEFT"){
						player->setCurrentState(L"JUMPING_ASCEND_RIGHT");
						player->setFacingRight(true);
					}

				}
				else
				{
					if (!player->isAirborne()){
						vX = 0.0f;
					}

					if (state == L"WALK_LEFT" || state == L"IDLE_LEFT"){
						player->setCurrentState(L"IDLE_LEFT");
					}
					else if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT"){
						player->setCurrentState(L"IDLE_RIGHT");
					}
				}
			}
			else
			{
				if (!player->isAirborne()){
					vX = 0.0f;
				}

				if (state == L"WALK_LEFT" || state == L"IDLE_LEFT"){
					player->setCurrentState(L"IDLE_LEFT");
				}
				else if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT"){
					player->setCurrentState(L"IDLE_RIGHT");
				}
			}

			if (input->isKeyDownForFirstTime(SPACE_KEY))
			{
				if (state == L"WALK_LEFT" || state == L"IDLE_LEFT"
					|| state == L"WALK_RIGHT" || state == L"IDLE_RIGHT"
					|| player->isAttacking()){
					if (!player->isAirborne()){
						vY = JUMP_SPEED;
						player->setWasJump(true);
						player->setAirborne(true);
					}
				}
			}
			//FOR PRECISION JUMPING, halts ascent when jump key is released
			if (input->wasKeyReleased(SPACE_KEY)){
				if (vY > 0 && player->wasJump()){
					vY = vY / 3.0f;
				}
				player->setWasJump(false);
			}

			//ATTACKS
			if (input->isKeyDownForFirstTime(K_KEY)){
				if (!player->wasHit()){
					b2BodyDef hurtBoxProps;
					b2FixtureDef fixtureDef;
					b2PolygonShape shape;

					hurtBoxProps.fixedRotation = true;
					fixtureDef.isSensor = true;

					boolean dPressed = input->isKeyDown(D_KEY);
					boolean aPressed = input->isKeyDown(A_KEY);
					//Animation Handling
					if (state == L"ATTACK_RIGHT" || state == L"ATTACK_LEFT"){
						if (dPressed && !aPressed){
							player->setCurrentState(L"ATTACK_RIGHT_2");
							player->setFacingRight(true);
						}
						else if (aPressed && !dPressed){
							player->setCurrentState(L"ATTACK_LEFT_2");
							player->setFacingRight(false);
						}
						else{
							if (state == L"ATTACK_RIGHT"){
								player->setCurrentState(L"ATTACK_RIGHT_2");
							}
							else{
								player->setCurrentState(L"ATTACK_LEFT_2");
							}
						}
					}
					else if (state == L"ATTACK_RIGHT_2" || state == L"ATTACK_LEFT_2"){
						if (dPressed && !aPressed){
							player->setCurrentState(L"ATTACK_RIGHT");
							player->setFacingRight(true);
						}
						else if (aPressed && !dPressed){
							player->setCurrentState(L"ATTACK_LEFT");
							player->setFacingRight(false);
						}
						else{
							if (state == L"ATTACK_RIGHT_2"){
								player->setCurrentState(L"ATTACK_RIGHT");
							}
							else{
								player->setCurrentState(L"ATTACK_LEFT");
							}
						}
					}
					else if (player->isFacingRight()){
						player->setCurrentState(L"ATTACK_RIGHT");
					}
					else{
						player->setCurrentState(L"ATTACK_LEFT");
					}

					if (!player->getHurtBox()->IsActive()){
						player->getHurtBox()->SetActive(true);
					}
					player->setAttacking(true);
				}
			}

			//Hit testing
			if (input->isKeyDownForFirstTime(H_KEY)){
				if (player->isFacingRight()){
					player->setCurrentState(L"HIT_LEFT");
					vX = -3.0f;
				}
				else{
					player->setCurrentState(L"HIT_RIGHT");
					vX = 3.0f;
				}
				vY = 8.0f;
				player->setHit(true);
				player->decrementHP();
			}

			//for testing HP and death
			/*if (input->isKeyDownForFirstTime(K_KEY)){
				AnimatedSprite* player = gsm->getSpriteManager()->getPlayer();
				if (player->getHP() != 1){
					player->decrementHP();
				}
				else{
					player->decrementHP();
					player->toggleControllable();
					vX = 0.0f;
					player->setCurrentState(L"DIE");
					player->setVisibleFrames(100);
				}
			}*/

		}
		if (input->isKeyDownForFirstTime(G_KEY))
		{
			viewport->toggleDebugView();
			game->getGraphics()->toggleDebugTextShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(P_KEY))
		{
			gsm->getPhysics()->togglePhysics();
		}
		if (input->isKeyDownForFirstTime(T_KEY))
		{
			gsm->getPhysics()->activateForSingleUpdate();
		}
		// NOW SET THE ACTUAL PLAYER VELOCITY
		pp->SetLinearVelocity(b2Vec2(vX, vY));


		//Since the viewport being moved is reliant on some player input, its handled here
		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		float viewportX = viewport->getViewportX() - viewport->getViewportOffsetX() + viewport->getViewportWidth() / 2.0f;
		float viewportY = viewport->getViewportY() - viewport->getViewportOffsetY() + viewport->getViewportHeight() / 2.0f;
		//We're removing the ability to move the viewport manually.
		/*if (input->isKeyDown(UP_KEY))
		{
			viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(DOWN_KEY))
		{
			viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(LEFT_KEY))
		{
			viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(RIGHT_KEY))
		{
			viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}*/
		Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved){
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		}
		//Recenters viewport back on player if no camera keys are being pressed
		else{
			float diff;
			float height = (float)gsm->getWorld()->getWorldHeight();
			//IF PLAYER IS FACING RIGHT, VIEWPORT SHOULD BE MORE RIGHT
			//ELSE, MORE LEFT
			if (player->isFacingRight()){
				if (pp->GetPosition().x*METER_TO_PIXEL_SCALE > viewportX - 240){
					diff = (pp->GetPosition().x*METER_TO_PIXEL_SCALE - viewportX + 240) / 7;
					if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
						viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
					}
					else{
						viewportVx += diff;
					}
				}
				else if (pp->GetPosition().x*METER_TO_PIXEL_SCALE < viewportX - 240){
					diff = (viewportX - pp->GetPosition().x*METER_TO_PIXEL_SCALE - 240) / 7;
					if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
						viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
					}
					else{
						viewportVx -= diff;
					}
				}
			}
			else{
				if (pp->GetPosition().x*METER_TO_PIXEL_SCALE > viewportX){
					diff = (pp->GetPosition().x*METER_TO_PIXEL_SCALE - viewportX) / 7;
					if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
						viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
					}
					else{
						viewportVx += diff;
					}
				}
				else if (pp->GetPosition().x*METER_TO_PIXEL_SCALE < viewportX){
					diff = (viewportX - pp->GetPosition().x*METER_TO_PIXEL_SCALE) / 7;
					if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
						viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
					}
					else{
						viewportVx -= diff;
					}
				}
			}
			

			if ((height - (pp->GetPosition().y*METER_TO_PIXEL_SCALE)) < viewportY){
				diff = (viewportY - (height - (pp->GetPosition().y*METER_TO_PIXEL_SCALE))) / 7;
				if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
					viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
				}
				else{
					viewportVy -= diff;
				}
			}
			else if ((height - (pp->GetPosition().y*METER_TO_PIXEL_SCALE)) > viewportY){
				diff = ((height - (pp->GetPosition().y*METER_TO_PIXEL_SCALE)) - viewportY) / 7;
				if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
					viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
				}
				else{
					viewportVy += diff;
				}
			}
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		}
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}