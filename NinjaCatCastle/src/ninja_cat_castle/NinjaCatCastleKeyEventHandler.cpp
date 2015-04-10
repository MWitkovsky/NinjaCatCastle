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
	b2BodyDef *pp = &player->getBodyDef();
	Viewport *viewport = game->getGUI()->getViewport();
	
	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->linearVelocity.x;
		float vY = -pp->linearVelocity.y;
		wstring state = player->getCurrentState();

		// YOU MIGHT WANT TO UNCOMMENT THIS FOR SOME TESTING,
		// BUT IN THIS ASSIGNMENT, THE USER MOVES VIA MOUSE BUTTON PRESSES
		if (player->isControllable()){
			if (input->isKeyDown(A_KEY))
			{
				if (state != L"JUMPING_DESCEND_LEFT" && state != L"JUMPING_DESCEND_RIGHT"
					&& state != L"JUMPING_ASCEND_LEFT" && state != L"JUMPING_ASCEND_RIGHT"
					&& state != L"JUMPING_ARC_LEFT" && state != L"JUMPING_ARC_RIGHT"){
					player->setFacingRight(false);
					player->setCurrentState(L"WALK_LEFT");
					vX = -PLAYER_SPEED;
				}
			}
			else if (input->isKeyDown(D_KEY))
			{
				if (state != L"JUMPING_DESCEND_LEFT" && state != L"JUMPING_DESCEND_RIGHT"
					&& state != L"JUMPING_ASCEND_LEFT" && state != L"JUMPING_ASCEND_RIGHT"
					&& state != L"JUMPING_ARC_LEFT" && state != L"JUMPING_ARC_RIGHT"){
					player->setFacingRight(true);
					player->setCurrentState(L"WALK_RIGHT");
					vX = PLAYER_SPEED;
				}
			}
			else
			{
				if (state == L"WALK_LEFT" || state == L"IDLE_LEFT"){
					vX = 0.0f;
					player->setCurrentState(L"IDLE_LEFT");
				}
				else if (state == L"WALK_RIGHT" || state == L"IDLE_RIGHT"){
					vX = 0.0f;
					player->setCurrentState(L"IDLE_RIGHT");
				}
			}

			if (input->isKeyDownForFirstTime(SPACE_KEY))
			{
				if (player->wasOnTileLastFrame()){
					vY = JUMP_SPEED;
					player->setWasJump(true);
				}
			}

			//FOR PRECISION JUMPING
			if (input->wasKeyReleased(SPACE_KEY)){
				if (vY < 0){
					vY = vY / 3.0f;
				}
				player->setWasJump(false);
			}

			//for testing HP and death
			if (input->isKeyDownForFirstTime(K_KEY)){
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
			}
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
		pp->linearVelocity.x = vX;
		pp->linearVelocity.y = vY;

		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		float viewportX = viewport->getViewportX() - viewport->getViewportOffsetX() + viewport->getViewportWidth() / 2.0f;
		float viewportY = viewport->getViewportY() - viewport->getViewportOffsetY() + viewport->getViewportHeight() / 2.0f;
		if (input->isKeyDown(UP_KEY))
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
		}
		Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved){
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		}
		//Recenters viewport back on player if no camera keys are being pressed
		else{
			float diff;
			if (pp->position.x > viewportX - 60){
				diff = (pp->position.x - viewportX + 60) / 7;
				if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
					viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
				}
				else{
					viewportVx += diff;
				}
			}
			else if (pp->position.x < viewportX - 60){
				diff = (viewportX - pp->position.x - 60) / 7;
				if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
					viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
				}
				else{
					viewportVx -= diff;
				}
			}
			if (pp->position.y < viewportY){
				diff = (viewportY - pp->position.y) / 7;
				if (diff > MAX_VIEWPORT_AXIS_VELOCITY){
					viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
				}
				else{
					viewportVy -= diff;
				}
			}
			else if (pp->position.y > viewportY){
				diff = (pp->position.y - viewportY) / 7;
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

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;		
		cursor->setActiveCursorID(id);
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