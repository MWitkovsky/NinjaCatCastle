#pragma once
#include "ninja_cat_castle_VS\stdafx.h"

/*
	NinjaCatCastleGame.h

	This file contains setup data for making the NinjaCatCastle game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE NinjaCatCastle GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	W_LEVEL_1_DIR			= L"data/levels/SideScrollerMetal/";
static const wstring	W_LEVEL_1_NAME			= L"SideScrollerMetalLevel.tmx";
static const wstring	W_LEVEL_2_DIR			= L"data/levels/SideScrollerDesert/";
static const wstring	W_LEVEL_2_NAME			= L"SideScrollerDesertLevel.tmx";
static const wstring	W_LEVEL_1_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const wstring	W_INIT_FILE				= L"data/NinjaCatCastleInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/gui/NinjaCatCastleGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE					= L"TITLE";
static const wstring	W_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT			= L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X		= L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y		= L"VIEWPORT_OFFSET_Y";
static const wstring	W_TOGGLE_OFFSET_Y		= L"TOGGLE_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED			= L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_START_COMMAND			= L"Start";
// Ryota Shimamura 2015/03/30 ->
static const wstring	W_HELP_COMMAND			= L"Help";
static const wstring    W_ABOUT_COMMAND			= L"About";
// Ryota Shimamura 2015 / 03 / 30 <-
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_CURSOR_PATH			= L"data/gui/cursor/kunai_cursor.png";
// Ryota Shimamura 2015/03/30 ->
static const wstring	W_SPLASH_SCREEN_PATH	= L"data/gui/overlays/ninja_cat_castle_screen_overlay.png";
static const wstring	W_MAIN_MENU_PATH		= L"data/gui/overlays/ninja_cat_castle_main_menu_overlay.png";
static const wstring	W_HELP_SCREEN_PATH		= L"data/gui/overlays/ninja_cat_castle_help_screen.png";
static const wstring	W_ABOUT_SCREEN_PATH		= L"data/gui/overlays/ninja_cat_castle_about_screen.png";
static const wstring	W_HELP_IMAGE_PATH		= L"data/gui/buttons/help_game.png";
static const wstring	W_HELP_IMAGE_MO_PATH	= L"data/gui/buttons/help_game_mo.png";
static const wstring	W_ABOUT_IMAGE_PATH		= L"data/gui/buttons/about_game.png";
static const wstring	W_ABOUT_IMAGE_MO_PATH	= L"data/gui/buttons/about_game_mo.png";
// Ryota Shimamura 2015/03/30 <-
static const wstring	W_START_IMAGE_PATH		= L"data/gui/buttons/start_game.png";
static const wstring	W_START_IMAGE_MO_PATH	= L"data/gui/buttons/start_game_mo.png";
static const wstring	W_EXIT_IMAGE_PATH		= L"data/gui/buttons/exit_game.png";
static const wstring	W_EXIT_IMAGE_MO_PATH	= L"data/gui/buttons/exit_game_mo.png";
static const wstring	W_QUIT_IMAGE_PATH		= L"data/gui/buttons/quit.png";
static const wstring	W_QUIT_IMAGE_MO_PATH	= L"data/gui/buttons/quit_mo.png";
static const wstring	W_LIVES_HEAD_PATH		= L"data/gui/HUD/lives_head.png";

// FOR OUR SPRITE
// ...kind of neglected to bother listing them all here. oops.
// I like how the states stand out as a different color anyway. Easier to read.
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";
static const wstring	JUMPING = L"JUMPING";
static const wstring	ATTACKING_RIGHT = L"ATTACKING_RIGHT";
static const wstring	ATTACKING_LEFT = L"ATTACKING_LEFT";
static const wstring	IDLE = L"IDLE";
static const wstring    DIE = L"DIE";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 30.0f;
static const float		W_GRAVITY = 3.2f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 40;
static const float		PLAYER_INIT_X = 80.0f;
static const float		PLAYER_INIT_Y = 1000.0f;
static const float		JUMP_SPEED = 12.0f;
static const float		PLAYER_SPEED = 7.5f;
static const float		PLAYER_MELEE_RANGE = 0.5f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 60;
static const int		FPS_INC = 1;
static const int		START_BUTTON_X = 375;
static const int		START_BUTTON_Y = 50;
static const int		METER_TO_PIXEL_SCALE = 64;

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int G_KEY = (unsigned int)'G';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int K_KEY = (unsigned int)'K';
const unsigned int J_KEY = (unsigned int)'J';
const unsigned int H_KEY = (unsigned int)'H';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';