/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.cpp

	See Game.h for a class description.
*/
#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Button.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"
#include "sssf\text\TextGenerator.h"
#include "sssf\timer\GameTimer.h"

float MAX_VIEWPORT_AXIS_VELOCITY;
float W_GRAVITY;
float JUMP_SPEED;
float PLAYER_SPEED;
float PLAYER_MELEE_RANGE;

int MIN_FPS;
int	MAX_FPS;
int	FPS_INC;
int	LIVES_LABEL_X;
int	LIVES_LABEL_Y;
int	LIVES_COUNTER_X;
int	LIVES_COUNTER_Y;
int	SHURIKEN_LABEL_X;
int	SHURIKEN_LABEL_Y;
int SHURIKEN_COUNTER_X;
int	SHURIKEN_COUNTER_Y;
int	TREATS_LABEL_X;
int	TREATS_LABEL_Y;
int	TREATS_COUNTER_X;
int	TREATS_COUNTER_Y;
int	START_BUTTON_X;
int	START_BUTTON_Y;
int LIVES_HEAD_X;
int LIVES_HEAD_Y;
int	SHURIKEN_ICON_X;
int	SHURIKEN_ICON_Y;
int	TREAT_ICON_X;
int	TREAT_ICON_Y;
int	METER_TO_PIXEL_SCALE;

char* SPLASH_JINGLE;
char* MAIN_MENU_SONG_INTRO;
char* MAIN_MENU_SONG;
char* LEVEL_1_SONG_INTRO;
char* LEVEL_1_SONG;
char* LEVEL_2_SONG_INTRO;
char* LEVEL_2_SONG;
char* LEVEL_3_SONG_INTRO;
char* LEVEL_3_SONG;
char* LEVEL_COMPLETE_JINGLE;
char* ONE_UP_JINGLE;

char*	SOUND_ARROWSHOT;
char*	SOUND_ARMORCAT_BLOCK;
char*	SOUND_ARMORCAT_DIE;
char*	SOUND_BOMB_EXPLOSION1;
char*	SOUND_BOMB_EXPLOSION2;
char*	SOUND_CAT_POUNCE;
char*	SOUND_PROPELLER;
char*	SOUND_HIT;
char*	SOUND_HIT2;
char*	SOUND_MC_DEATH;
char*	SOUND_MC_JUMP;
char*	SOUND_SLASH1;
char*	SOUND_SLASH2;
char*	SOUND_BIGSLASH;
char*	SOUND_PICKUP_TREAT;
char*	SOUND_PICKUP_HEALTH;
char*	SOUND_MEOW;

wstring W_LEVEL_1_DIR;
wstring W_LEVEL_1_NAME;
wstring W_LEVEL_2_DIR;
wstring W_LEVEL_2_NAME;
wstring W_LEVEL_3_DIR;
wstring W_LEVEL_3_NAME;
wstring W_INIT_FILE;
wstring W_GUI_INIT_FILE;
wstring W_LEVEL_1_PATH;
wstring W_TITLE;
wstring W_SCREEN_WIDTH;
wstring W_SCREEN_HEIGHT;
wstring W_USE_FULLSCREEN_MODE;
wstring W_TEXT_FONT_SIZE;
wstring W_VIEWPORT_OFFSET_X;
wstring W_VIEWPORT_OFFSET_Y;
wstring W_TOGGLE_OFFSET_Y;
wstring W_FONT_COLOR_RED;
wstring W_FONT_COLOR_GREEN;
wstring W_FONT_COLOR_BLUE;
wstring W_COLOR_KEY_RED;
wstring W_COLOR_KEY_GREEN;
wstring W_COLOR_KEY_BLUE;
string W_DEBUG_FILE;

wstring	W_GO_TO_MM_COMMAND;
wstring	W_START_COMMAND;
wstring	W_HELP_COMMAND;
wstring W_ABOUT_COMMAND;
wstring	W_EXIT_COMMAND;
wstring	W_QUIT_COMMAND;
wstring	W_CURSOR_PATH;
wstring	W_SPLASH_SCREEN_PATH;
wstring	W_MAIN_MENU_PATH;
wstring	W_HELP_SCREEN_PATH;
wstring	W_ABOUT_SCREEN_PATH;
wstring	W_HELP_IMAGE_PATH;
wstring	W_HELP_IMAGE_MO_PATH;
wstring	W_ABOUT_IMAGE_PATH;
wstring	W_ABOUT_IMAGE_MO_PATH;
wstring	W_START_IMAGE_PATH;
wstring	W_START_IMAGE_MO_PATH;
wstring	W_EXIT_IMAGE_PATH;
wstring	W_EXIT_IMAGE_MO_PATH;
wstring	W_QUIT_IMAGE_PATH;
wstring	W_QUIT_IMAGE_MO_PATH;
wstring	W_LIVES_HEAD_PATH;
wstring	W_SHURIKEN_ICON_PATH;
wstring	W_TREAT_ICON_PATH;

wstring	SPRITE_TYPES_DIR;
wstring	SPRITE_TYPES_LIST;

FMOD::System*		fmodSystem; //handle to FMOD engine
FMOD::Channel*		musicChannel;
FMOD::Channel*		introChannel; //needed for seamless looping
int musicDelay;

//ALL GLOBAL SCOPE VARIABLES ARE LOADED HERE FROM THE LUA FILE IN THE DATA DIRECTORY
void Game::readLUA(const char* fileName){
	//LUA STUFF WOO

	// Init Lua
	LuaState* luaPState = LuaState::Create();

	// Open the Lua Script File
	int result = luaPState->DoFile(fileName);

	// Get a global variable  
	LuaObject MAX_VIEWPORT_AXIS_VELOCITY_obj = luaPState->GetGlobal("MAX_VIEWPORT_AXIS_VELOCITY");
	MAX_VIEWPORT_AXIS_VELOCITY = MAX_VIEWPORT_AXIS_VELOCITY_obj.GetFloat();

	LuaObject W_GRAVITY_obj = luaPState->GetGlobal("W_GRAVITY");
	W_GRAVITY = W_GRAVITY_obj.GetFloat();

	LuaObject JUMP_SPEED_obj = luaPState->GetGlobal("JUMP_SPEED");
	JUMP_SPEED = JUMP_SPEED_obj.GetFloat();

	LuaObject PLAYER_SPEED_obj = luaPState->GetGlobal("PLAYER_SPEED");
	PLAYER_SPEED = PLAYER_SPEED_obj.GetFloat();

	LuaObject PLAYER_MELEE_RANGE_obj = luaPState->GetGlobal("PLAYER_MELEE_RANGE");
	PLAYER_MELEE_RANGE = PLAYER_MELEE_RANGE_obj.GetFloat();


	LuaObject MIN_FPS_obj = luaPState->GetGlobal("MIN_FPS");
	MIN_FPS = MIN_FPS_obj.GetInteger();

	LuaObject MAX_FPS_obj = luaPState->GetGlobal("MAX_FPS");
	MAX_FPS = MAX_FPS_obj.GetInteger();

	LuaObject FPS_INC_obj = luaPState->GetGlobal("FPS_INC");
	FPS_INC = FPS_INC_obj.GetInteger();

	LuaObject LIVES_LABEL_X_obj = luaPState->GetGlobal("LIVES_LABEL_X");
	LIVES_LABEL_X = LIVES_LABEL_X_obj.GetInteger();

	LuaObject LIVES_LABEL_Y_obj = luaPState->GetGlobal("LIVES_LABEL_Y");
	LIVES_LABEL_Y = LIVES_LABEL_Y_obj.GetInteger();

	LuaObject LIVES_COUNTER_X_obj = luaPState->GetGlobal("LIVES_COUNTER_X");
	LIVES_COUNTER_X = LIVES_COUNTER_X_obj.GetInteger();

	LuaObject LIVES_COUNTER_Y_obj = luaPState->GetGlobal("LIVES_COUNTER_Y");
	LIVES_COUNTER_Y = LIVES_COUNTER_Y_obj.GetInteger();

	LuaObject SHURIKEN_LABEL_X_obj = luaPState->GetGlobal("SHURIKEN_LABEL_X");
	SHURIKEN_LABEL_X = SHURIKEN_LABEL_X_obj.GetInteger();

	LuaObject SHURIKEN_LABEL_Y_obj = luaPState->GetGlobal("SHURIKEN_LABEL_Y");
	SHURIKEN_LABEL_Y = SHURIKEN_LABEL_Y_obj.GetInteger();

	LuaObject SHURIKEN_COUNTER_X_obj = luaPState->GetGlobal("SHURIKEN_COUNTER_X");
	SHURIKEN_COUNTER_X = SHURIKEN_COUNTER_X_obj.GetInteger();

	LuaObject SHURIKEN_COUNTER_Y_obj = luaPState->GetGlobal("SHURIKEN_COUNTER_Y");
	SHURIKEN_COUNTER_Y = SHURIKEN_COUNTER_Y_obj.GetInteger();

	LuaObject TREATS_LABEL_X_obj = luaPState->GetGlobal("TREATS_LABEL_X");
	TREATS_LABEL_X = TREATS_LABEL_X_obj.GetInteger();

	LuaObject TREATS_LABEL_Y_obj = luaPState->GetGlobal("TREATS_LABEL_Y");
	TREATS_LABEL_Y = TREATS_LABEL_Y_obj.GetInteger();

	LuaObject TREATS_COUNTER_X_obj = luaPState->GetGlobal("TREATS_COUNTER_X");
	TREATS_COUNTER_X = TREATS_COUNTER_X_obj.GetInteger();

	LuaObject TREATS_COUNTER_Y_obj = luaPState->GetGlobal("TREATS_COUNTER_Y");
	TREATS_COUNTER_Y = TREATS_COUNTER_Y_obj.GetInteger();

	LuaObject START_BUTTON_X_obj = luaPState->GetGlobal("START_BUTTON_X");
	START_BUTTON_X = START_BUTTON_X_obj.GetInteger();

	LuaObject START_BUTTON_Y_obj = luaPState->GetGlobal("START_BUTTON_Y");
	START_BUTTON_Y = START_BUTTON_Y_obj.GetInteger();

	LuaObject LIVES_HEAD_X_obj = luaPState->GetGlobal("LIVES_HEAD_X");
	LIVES_HEAD_X = LIVES_HEAD_X_obj.GetInteger();

	LuaObject LIVES_HEAD_Y_obj = luaPState->GetGlobal("LIVES_HEAD_Y");
	LIVES_HEAD_Y = LIVES_HEAD_Y_obj.GetInteger();

	LuaObject SHURIKEN_ICON_X_obj = luaPState->GetGlobal("SHURIKEN_ICON_X");
	SHURIKEN_ICON_X = SHURIKEN_ICON_X_obj.GetInteger();

	LuaObject SHURIKEN_ICON_Y_obj = luaPState->GetGlobal("SHURIKEN_ICON_Y");
	SHURIKEN_ICON_Y = SHURIKEN_ICON_Y_obj.GetInteger();

	LuaObject TREAT_ICON_X_obj = luaPState->GetGlobal("TREAT_ICON_X");
	TREAT_ICON_X = TREAT_ICON_X_obj.GetInteger();

	LuaObject TREAT_ICON_Y_obj = luaPState->GetGlobal("TREAT_ICON_Y");
	TREAT_ICON_Y = TREAT_ICON_Y_obj.GetInteger();

	LuaObject METER_TO_PIXEL_SCALE_obj = luaPState->GetGlobal("METER_TO_PIXEL_SCALE");
	METER_TO_PIXEL_SCALE = METER_TO_PIXEL_SCALE_obj.GetInteger();

	//MUSIC AND SOUNDS//
	LuaObject SPLASH_JINGLE_obj = luaPState->GetGlobal("SPLASH_JINGLE");
	string splash_jingle = SPLASH_JINGLE_obj.GetString();
	SPLASH_JINGLE = strcpy((char*)malloc(splash_jingle.length() + 1), splash_jingle.c_str());

	LuaObject MAIN_MENU_SONG_INTRO_obj = luaPState->GetGlobal("MAIN_MENU_SONG_INTRO");
	string main_menu_song_intro = MAIN_MENU_SONG_INTRO_obj.GetString();
	MAIN_MENU_SONG_INTRO = strcpy((char*)malloc(main_menu_song_intro.length() + 1), main_menu_song_intro.c_str());

	LuaObject MAIN_MENU_SONG_obj = luaPState->GetGlobal("MAIN_MENU_SONG");
	string main_menu_song = MAIN_MENU_SONG_obj.GetString();
	MAIN_MENU_SONG = strcpy((char*)malloc(main_menu_song.length() + 1), main_menu_song.c_str());

	LuaObject LEVEL_1_SONG_INTRO_obj = luaPState->GetGlobal("LEVEL_1_SONG_INTRO");
	string level_1_song_intro = LEVEL_1_SONG_INTRO_obj.GetString();
	LEVEL_1_SONG_INTRO = strcpy((char*)malloc(level_1_song_intro.length() + 1), level_1_song_intro.c_str());

	LuaObject LEVEL_1_SONG_obj = luaPState->GetGlobal("LEVEL_1_SONG");
	string level_1_song = LEVEL_1_SONG_obj.GetString();
	LEVEL_1_SONG = strcpy((char*)malloc(level_1_song.length() + 1), level_1_song.c_str());

	LuaObject LEVEL_2_SONG_INTRO_obj = luaPState->GetGlobal("LEVEL_2_SONG_INTRO");
	string level_2_song_intro = LEVEL_2_SONG_INTRO_obj.GetString();
	LEVEL_2_SONG_INTRO = strcpy((char*)malloc(level_2_song_intro.length() + 1), level_2_song_intro.c_str());

	LuaObject LEVEL_2_SONG_obj = luaPState->GetGlobal("LEVEL_2_SONG");
	string level_2_song = LEVEL_2_SONG_obj.GetString();
	LEVEL_2_SONG = strcpy((char*)malloc(level_2_song.length() + 1), level_2_song.c_str());

	LuaObject LEVEL_3_SONG_INTRO_obj = luaPState->GetGlobal("LEVEL_3_SONG_INTRO");
	string level_3_song_intro = LEVEL_3_SONG_INTRO_obj.GetString();
	LEVEL_3_SONG_INTRO = strcpy((char*)malloc(level_3_song_intro.length() + 1), level_3_song_intro.c_str());

	LuaObject LEVEL_3_SONG_obj = luaPState->GetGlobal("LEVEL_3_SONG");
	string level_3_song = LEVEL_3_SONG_obj.GetString();
	LEVEL_3_SONG = strcpy((char*)malloc(level_3_song.length() + 1), level_3_song.c_str());

	LuaObject LEVEL_COMPLETE_JINGLE_obj = luaPState->GetGlobal("LEVEL_COMPLETE_JINGLE");
	string level_complete_jingle = LEVEL_COMPLETE_JINGLE_obj.GetString();
	LEVEL_COMPLETE_JINGLE = strcpy((char*)malloc(level_complete_jingle.length() + 1), level_complete_jingle.c_str());

	LuaObject ONE_UP_JINGLE_obj = luaPState->GetGlobal("ONE_UP_JINGLE");
	string one_up_jingle = ONE_UP_JINGLE_obj.GetString();
	ONE_UP_JINGLE = strcpy((char*)malloc(one_up_jingle.length() + 1), one_up_jingle.c_str());

	LuaObject SOUND_ARMORCAT_BLOCK_obj = luaPState->GetGlobal("SOUND_ARMORCAT_BLOCK");
	string sound_armorcat_block = SOUND_ARMORCAT_BLOCK_obj.GetString();
	SOUND_ARMORCAT_BLOCK = strcpy((char*)malloc(sound_armorcat_block.length() + 1), sound_armorcat_block.c_str());

	LuaObject SOUND_ARMORCAT_DIE_obj = luaPState->GetGlobal("SOUND_ARMORCAT_DIE");
	string sound_armorcat_die = SOUND_ARMORCAT_DIE_obj.GetString();
	SOUND_ARMORCAT_DIE = strcpy((char*)malloc(sound_armorcat_die.length() + 1), sound_armorcat_die.c_str());

	LuaObject SOUND_ARROWSHOT_obj = luaPState->GetGlobal("SOUND_ARROWSHOT");
	string sound_arrowshot = SOUND_ARROWSHOT_obj.GetString();
	SOUND_ARROWSHOT = strcpy((char*)malloc(sound_arrowshot.length() + 1), sound_arrowshot.c_str());

	LuaObject SOUND_BOMB_EXPLOSION1_obj = luaPState->GetGlobal("SOUND_BOMB_EXPLOSION1");
	string sound_bomb_explosion1 = SOUND_BOMB_EXPLOSION1_obj.GetString();
	SOUND_BOMB_EXPLOSION1 = strcpy((char*)malloc(sound_bomb_explosion1.length() + 1), sound_bomb_explosion1.c_str());

	LuaObject SOUND_BOMB_EXPLOSION2_obj = luaPState->GetGlobal("SOUND_BOMB_EXPLOSION2");
	string sound_bomb_explosion2 = SOUND_BOMB_EXPLOSION1_obj.GetString();
	SOUND_BOMB_EXPLOSION2 = strcpy((char*)malloc(sound_bomb_explosion2.length() + 1), sound_bomb_explosion2.c_str());

	LuaObject SOUND_CAT_POUNCE_obj = luaPState->GetGlobal("SOUND_CAT_POUNCE");
	string sound_cat_pounce = SOUND_CAT_POUNCE_obj.GetString();
	SOUND_CAT_POUNCE = strcpy((char*)malloc(sound_cat_pounce.length() + 1), sound_cat_pounce.c_str());

	LuaObject SOUND_PROPELLER_obj = luaPState->GetGlobal("SOUND_PROPELLER");
	string sound_propeller = SOUND_PROPELLER_obj.GetString();
	SOUND_PROPELLER = strcpy((char*)malloc(sound_propeller.length() + 1), sound_propeller.c_str());

	LuaObject SOUND_HIT_obj = luaPState->GetGlobal("SOUND_HIT");
	string sound_hit = SOUND_HIT_obj.GetString();
	SOUND_HIT = strcpy((char*)malloc(sound_hit.length() + 1), sound_hit.c_str());

	LuaObject SOUND_HIT2_obj = luaPState->GetGlobal("SOUND_HIT2");
	string sound_hit2 = SOUND_HIT_obj.GetString();
	SOUND_HIT2 = strcpy((char*)malloc(sound_hit2.length() + 1), sound_hit2.c_str());

	LuaObject SOUND_MC_DEATH_obj = luaPState->GetGlobal("SOUND_MC_DEATH");
	string sound_mc_death = SOUND_MC_DEATH_obj.GetString();
	SOUND_MC_DEATH = strcpy((char*)malloc(sound_mc_death.length() + 1), sound_mc_death.c_str());

	LuaObject SOUND_MC_JUMP_obj = luaPState->GetGlobal("SOUND_MC_JUMP");
	string sound_mc_jump = SOUND_MC_JUMP_obj.GetString();
	SOUND_MC_JUMP = strcpy((char*)malloc(sound_mc_jump.length() + 1), sound_mc_jump.c_str());

	LuaObject SOUND_SLASH1_obj = luaPState->GetGlobal("SOUND_SLASH1");
	string sound_slash1 = SOUND_SLASH1_obj.GetString();
	SOUND_SLASH1 = strcpy((char*)malloc(sound_slash1.length() + 1), sound_slash1.c_str());

	LuaObject SOUND_SLASH2_obj = luaPState->GetGlobal("SOUND_SLASH2");
	string sound_slash2 = SOUND_SLASH2_obj.GetString();
	SOUND_SLASH2 = strcpy((char*)malloc(sound_slash2.length() + 1), sound_slash2.c_str());

	LuaObject SOUND_BIGSLASH_obj = luaPState->GetGlobal("SOUND_BIGSLASH");
	string sound_bigslash = SOUND_BIGSLASH_obj.GetString();
	SOUND_BIGSLASH = strcpy((char*)malloc(sound_bigslash.length() + 1), sound_bigslash.c_str());

	LuaObject SOUND_PICKUP_TREAT_obj = luaPState->GetGlobal("SOUND_PICKUP_TREAT");
	string sound_pickup_treat = SOUND_PICKUP_TREAT_obj.GetString();
	SOUND_PICKUP_TREAT = strcpy((char*)malloc(sound_pickup_treat.length() + 1), sound_pickup_treat.c_str());

	LuaObject SOUND_PICKUP_HEALTH_obj = luaPState->GetGlobal("SOUND_PICKUP_HEALTH");
	string sound_pickup_health = SOUND_PICKUP_HEALTH_obj.GetString();
	SOUND_PICKUP_HEALTH = strcpy((char*)malloc(sound_pickup_health.length() + 1), sound_pickup_health.c_str());

	LuaObject SOUND_MEOW_obj = luaPState->GetGlobal("SOUND_MEOW");
	string sound_meow = SOUND_MEOW_obj.GetString();
	SOUND_MEOW = strcpy((char*)malloc(sound_meow.length() + 1), sound_meow.c_str());

	LuaObject W_LEVEL_1_DIR_obj = luaPState->GetGlobal("W_LEVEL_1_DIR");
	string w_level1_dir = W_LEVEL_1_DIR_obj.GetString();
	wstring wW_LEVEL_1_DIR(w_level1_dir.begin(), w_level1_dir.end());
	W_LEVEL_1_DIR = wW_LEVEL_1_DIR;

	LuaObject W_LEVEL_1_NAME_obj = luaPState->GetGlobal("W_LEVEL_1_NAME");
	string w_level_1_name = W_LEVEL_1_NAME_obj.GetString();
	wstring wW_LEVEL_1_NAME(w_level_1_name.begin(), w_level_1_name.end());
	W_LEVEL_1_NAME = wW_LEVEL_1_NAME;

	LuaObject W_LEVEL_2_DIR_obj = luaPState->GetGlobal("W_LEVEL_2_DIR");
	string w_level_2_dir = W_LEVEL_2_DIR_obj.GetString();
	wstring wW_LEVEL_2_DIR(w_level_2_dir.begin(), w_level_2_dir.end());
	W_LEVEL_2_DIR = wW_LEVEL_2_DIR;

	LuaObject W_LEVEL_2_NAME_obj = luaPState->GetGlobal("W_LEVEL_2_NAME");
	string w_level_2_name = W_LEVEL_2_NAME_obj.GetString();
	wstring wW_LEVEL_2_NAME(w_level_2_name.begin(), w_level_2_name.end());
	W_LEVEL_2_NAME = wW_LEVEL_2_NAME;

	LuaObject W_LEVEL_3_DIR_obj = luaPState->GetGlobal("W_LEVEL_3_DIR");
	string w_level_3_dir = W_LEVEL_3_DIR_obj.GetString();
	wstring wW_LEVEL_3_DIR(w_level_3_dir.begin(), w_level_3_dir.end());
	W_LEVEL_3_DIR = wW_LEVEL_3_DIR;

	LuaObject W_LEVEL_3_NAME_obj = luaPState->GetGlobal("W_LEVEL_3_NAME");
	string w_level_3_name = W_LEVEL_3_NAME_obj.GetString();
	wstring wW_LEVEL_3_NAME(w_level_3_name.begin(), w_level_3_name.end());
	W_LEVEL_3_NAME = wW_LEVEL_3_NAME;

	LuaObject W_INIT_FILE_obj = luaPState->GetGlobal("W_INIT_FILE");
	string w_init_file = W_INIT_FILE_obj.GetString();
	wstring wW_INIT_FILE(w_init_file.begin(), w_init_file.end());
	W_INIT_FILE = wW_INIT_FILE;

	LuaObject W_GUI_INIT_FILE_obj = luaPState->GetGlobal("W_GUI_INIT_FILE");
	string w_gui_init_file = W_GUI_INIT_FILE_obj.GetString();
	wstring wW_GUI_INIT_FILE(w_gui_init_file.begin(), w_gui_init_file.end());
	W_GUI_INIT_FILE = wW_GUI_INIT_FILE;

	LuaObject W_TITLE_obj = luaPState->GetGlobal("W_TITLE");
	string w_title = W_TITLE_obj.GetString();
	wstring wW_TITLE(w_title.begin(), w_title.end());
	W_TITLE = wW_TITLE;

	LuaObject W_SCREEN_WIDTH_obj = luaPState->GetGlobal("W_SCREEN_WIDTH");
	string w_screen_width = W_SCREEN_WIDTH_obj.GetString();
	wstring wW_SCREEN_WIDTH(w_screen_width.begin(), w_screen_width.end());
	W_SCREEN_WIDTH = wW_SCREEN_WIDTH;

	LuaObject W_SCREEN_HEIGHT_obj = luaPState->GetGlobal("W_SCREEN_HEIGHT");
	string w_screen_height = W_SCREEN_HEIGHT_obj.GetString();
	wstring wW_SCREEN_HEIGHT(w_screen_height.begin(), w_screen_height.end());
	W_SCREEN_HEIGHT = wW_SCREEN_HEIGHT;

	LuaObject W_USE_FULLSCREEN_MODE_obj = luaPState->GetGlobal("W_USE_FULLSCREEN_MODE");
	string w_use_fullscreen_mode = W_USE_FULLSCREEN_MODE_obj.GetString();
	wstring wW_USE_FULLSCREEN_MODE(w_use_fullscreen_mode.begin(), w_use_fullscreen_mode.end());
	W_USE_FULLSCREEN_MODE = wW_USE_FULLSCREEN_MODE;

	LuaObject W_TEXT_FONT_SIZE_obj = luaPState->GetGlobal("W_TEXT_FONT_SIZE");
	string w_text_font_size = W_TEXT_FONT_SIZE_obj.GetString();
	wstring wW_TEXT_FONT_SIZE(w_text_font_size.begin(), w_text_font_size.end());
	W_TEXT_FONT_SIZE = wW_TEXT_FONT_SIZE;

	LuaObject W_VIEWPORT_OFFSET_X_obj = luaPState->GetGlobal("W_VIEWPORT_OFFSET_X");
	string w_viewport_offset_x = W_VIEWPORT_OFFSET_X_obj.GetString();
	wstring wW_VIEWPORT_OFFSET_X(w_viewport_offset_x.begin(), w_viewport_offset_x.end());
	W_VIEWPORT_OFFSET_X = wW_VIEWPORT_OFFSET_X;

	LuaObject W_VIEWPORT_OFFSET_Y_obj = luaPState->GetGlobal("W_VIEWPORT_OFFSET_Y");
	string w_viewport_offset_y = W_VIEWPORT_OFFSET_Y_obj.GetString();
	wstring wW_VIEWPORT_OFFSET_Y(w_viewport_offset_y.begin(), w_viewport_offset_y.end());
	W_VIEWPORT_OFFSET_Y = wW_VIEWPORT_OFFSET_Y;

	LuaObject W_TOGGLE_OFFSET_Y_obj = luaPState->GetGlobal("W_TOGGLE_OFFSET_Y");
	string w_toggle_offset_y = W_TOGGLE_OFFSET_Y_obj.GetString();
	wstring wW_TOGGLE_OFFSET_Y(w_toggle_offset_y.begin(), w_toggle_offset_y.end());
	W_TOGGLE_OFFSET_Y = wW_TOGGLE_OFFSET_Y;

	LuaObject W_FONT_COLOR_RED_obj = luaPState->GetGlobal("W_FONT_COLOR_RED");
	string w_font_color_red = W_FONT_COLOR_RED_obj.GetString();
	wstring wW_FONT_COLOR_RED(w_font_color_red.begin(), w_font_color_red.end());
	W_FONT_COLOR_RED = wW_FONT_COLOR_RED;

	LuaObject W_FONT_COLOR_GREEN_obj = luaPState->GetGlobal("W_FONT_COLOR_GREEN");
	string w_font_color_green = W_FONT_COLOR_GREEN_obj.GetString();
	wstring wW_FONT_COLOR_GREEN(w_font_color_green.begin(), w_font_color_green.end());
	W_FONT_COLOR_GREEN = wW_FONT_COLOR_GREEN;

	LuaObject W_FONT_COLOR_BLUE_obj = luaPState->GetGlobal("W_FONT_COLOR_BLUE");
	string w_font_color_blue = W_FONT_COLOR_BLUE_obj.GetString();
	wstring wW_FONT_COLOR_BLUE(w_font_color_blue.begin(), w_font_color_blue.end());
	W_FONT_COLOR_BLUE = wW_FONT_COLOR_BLUE;

	LuaObject W_COLOR_KEY_RED_obj = luaPState->GetGlobal("W_COLOR_KEY_RED");
	string w_color_key_red = W_COLOR_KEY_RED_obj.GetString();
	wstring wW_COLOR_KEY_RED(w_color_key_red.begin(), w_color_key_red.end());
	W_COLOR_KEY_RED = wW_COLOR_KEY_RED;

	LuaObject W_COLOR_KEY_GREEN_obj = luaPState->GetGlobal("W_COLOR_KEY_GREEN");
	string w_color_key_green = W_COLOR_KEY_GREEN_obj.GetString();
	wstring wW_COLOR_KEY_GREEN(w_color_key_green.begin(), w_color_key_green.end());
	W_COLOR_KEY_GREEN = wW_COLOR_KEY_GREEN;

	LuaObject W_COLOR_KEY_BLUE_obj = luaPState->GetGlobal("W_COLOR_KEY_BLUE");
	string w_color_key_blue = W_COLOR_KEY_BLUE_obj.GetString();
	wstring wW_COLOR_KEY_BLUE(w_color_key_blue.begin(), w_color_key_blue.end());
	W_COLOR_KEY_BLUE = wW_COLOR_KEY_BLUE;

	LuaObject W_GO_TO_MM_COMMAND_obj = luaPState->GetGlobal("W_GO_TO_MM_COMMAND");
	string w_go_to_mm_command = W_GO_TO_MM_COMMAND_obj.GetString();
	wstring wW_GO_TO_MM_COMMAND(w_go_to_mm_command.begin(), w_go_to_mm_command.end());
	W_GO_TO_MM_COMMAND = wW_GO_TO_MM_COMMAND;

	LuaObject W_START_COMMAND_obj = luaPState->GetGlobal("W_START_COMMAND");
	string w_start_command = W_START_COMMAND_obj.GetString();
	wstring wW_START_COMMAND(w_start_command.begin(), w_start_command.end());
	W_START_COMMAND = wW_START_COMMAND;

	LuaObject W_HELP_COMMAND_obj = luaPState->GetGlobal("W_HELP_COMMAND");
	string w_help_command = W_HELP_COMMAND_obj.GetString();
	wstring wW_HELP_COMMAND(w_help_command.begin(), w_help_command.end());
	W_HELP_COMMAND = wW_HELP_COMMAND;

	LuaObject W_ABOUT_COMMAND_obj = luaPState->GetGlobal("W_ABOUT_COMMAND");
	string w_about_command = W_ABOUT_COMMAND_obj.GetString();
	wstring wW_ABOUT_COMMAND(w_about_command.begin(), w_about_command.end());
	W_ABOUT_COMMAND = wW_ABOUT_COMMAND;

	LuaObject W_EXIT_COMMAND_obj = luaPState->GetGlobal("W_EXIT_COMMAND");
	string w_exit_command = W_EXIT_COMMAND_obj.GetString();
	wstring wW_EXIT_COMMAND(w_exit_command.begin(), w_exit_command.end());
	W_EXIT_COMMAND = wW_EXIT_COMMAND;

	LuaObject W_QUIT_COMMAND_obj = luaPState->GetGlobal("W_QUIT_COMMAND");
	string w_quit_command = W_QUIT_COMMAND_obj.GetString();
	wstring wW_QUIT_COMMAND(w_quit_command.begin(), w_quit_command.end());
	W_QUIT_COMMAND = wW_QUIT_COMMAND;

	LuaObject W_CURSOR_PATH_obj = luaPState->GetGlobal("W_CURSOR_PATH");
	string w_cursor_path = W_CURSOR_PATH_obj.GetString();
	wstring wW_CURSOR_PATH(w_cursor_path.begin(), w_cursor_path.end());
	W_CURSOR_PATH = wW_CURSOR_PATH;

	LuaObject W_SPLASH_SCREEN_PATH_obj = luaPState->GetGlobal("W_SPLASH_SCREEN_PATH");
	string w_splash_screen_path = W_SPLASH_SCREEN_PATH_obj.GetString();
	wstring wW_SPLASH_SCREEN_PATH(w_splash_screen_path.begin(), w_splash_screen_path.end());
	W_SPLASH_SCREEN_PATH = wW_SPLASH_SCREEN_PATH;

	LuaObject W_MAIN_MENU_PATH_obj = luaPState->GetGlobal("W_MAIN_MENU_PATH");
	string w_main_menu_path = W_MAIN_MENU_PATH_obj.GetString();
	wstring wW_MAIN_MENU_PATH(w_main_menu_path.begin(), w_main_menu_path.end());
	W_MAIN_MENU_PATH = wW_MAIN_MENU_PATH;

	LuaObject W_HELP_SCREEN_PATH_obj = luaPState->GetGlobal("W_HELP_SCREEN_PATH");
	string w_help_screen_path = W_HELP_SCREEN_PATH_obj.GetString();
	wstring wW_HELP_SCREEN_PATH(w_help_screen_path.begin(), w_help_screen_path.end());
	W_HELP_SCREEN_PATH = wW_HELP_SCREEN_PATH;

	LuaObject W_ABOUT_SCREEN_PATH_obj = luaPState->GetGlobal("W_ABOUT_SCREEN_PATH");
	string w_about_screen_path = W_ABOUT_SCREEN_PATH_obj.GetString();
	wstring wW_ABOUT_SCREEN_PATH(w_about_screen_path.begin(), w_about_screen_path.end());
	W_ABOUT_SCREEN_PATH = wW_ABOUT_SCREEN_PATH;

	LuaObject W_HELP_IMAGE_PATH_obj = luaPState->GetGlobal("W_HELP_IMAGE_PATH");
	string w_help_image_path = W_HELP_IMAGE_PATH_obj.GetString();
	wstring wW_HELP_IMAGE_PATH(w_help_image_path.begin(), w_help_image_path.end());
	W_HELP_IMAGE_PATH = wW_HELP_IMAGE_PATH;

	LuaObject W_HELP_IMAGE_MO_PATH_obj = luaPState->GetGlobal("W_HELP_IMAGE_MO_PATH");
	string w_help_image_mo_path = W_HELP_IMAGE_MO_PATH_obj.GetString();
	wstring wW_HELP_IMAGE_MO_PATH(w_help_image_mo_path.begin(), w_help_image_mo_path.end());
	W_HELP_IMAGE_MO_PATH = wW_HELP_IMAGE_MO_PATH;

	LuaObject W_ABOUT_IMAGE_PATH_obj = luaPState->GetGlobal("W_ABOUT_IMAGE_PATH");
	string w_about_image_path = W_ABOUT_IMAGE_PATH_obj.GetString();
	wstring wW_ABOUT_IMAGE_PATH(w_about_image_path.begin(), w_about_image_path.end());
	W_ABOUT_IMAGE_PATH = wW_ABOUT_IMAGE_PATH;

	LuaObject W_ABOUT_IMAGE_MO_PATH_obj = luaPState->GetGlobal("W_ABOUT_IMAGE_MO_PATH");
	string w_about_image_mo_path = W_ABOUT_IMAGE_MO_PATH_obj.GetString();
	wstring wW_ABOUT_IMAGE_MO_PATH(w_about_image_mo_path.begin(), w_about_image_mo_path.end());
	W_ABOUT_IMAGE_MO_PATH = wW_ABOUT_IMAGE_MO_PATH;

	LuaObject W_START_IMAGE_PATH_obj = luaPState->GetGlobal("W_START_IMAGE_PATH");
	string w_start_image_path = W_START_IMAGE_PATH_obj.GetString();
	wstring wW_START_IMAGE_PATH(w_start_image_path.begin(), w_start_image_path.end());
	W_START_IMAGE_PATH = wW_START_IMAGE_PATH;

	LuaObject W_START_IMAGE_MO_PATH_obj = luaPState->GetGlobal("W_START_IMAGE_MO_PATH");
	string w_start_image_mo_path = W_START_IMAGE_MO_PATH_obj.GetString();
	wstring wW_START_IMAGE_MO_PATH(w_start_image_mo_path.begin(), w_start_image_mo_path.end());
	W_START_IMAGE_MO_PATH = wW_START_IMAGE_MO_PATH;

	LuaObject W_EXIT_IMAGE_PATH_obj = luaPState->GetGlobal("W_EXIT_IMAGE_PATH");
	string w_exit_image_path = W_EXIT_IMAGE_PATH_obj.GetString();
	wstring wW_EXIT_IMAGE_PATH(w_exit_image_path.begin(), w_exit_image_path.end());
	W_EXIT_IMAGE_PATH = wW_EXIT_IMAGE_PATH;

	LuaObject W_EXIT_IMAGE_MO_PATH_obj = luaPState->GetGlobal("W_EXIT_IMAGE_MO_PATH");
	string w_exit_image_mo_path = W_EXIT_IMAGE_MO_PATH_obj.GetString();
	wstring wW_EXIT_IMAGE_MO_PATH(w_exit_image_mo_path.begin(), w_exit_image_mo_path.end());
	W_EXIT_IMAGE_MO_PATH = wW_EXIT_IMAGE_MO_PATH;

	LuaObject W_QUIT_IMAGE_PATH_obj = luaPState->GetGlobal("W_QUIT_IMAGE_PATH");
	string w_quit_image_path = W_QUIT_IMAGE_PATH_obj.GetString();
	wstring wW_QUIT_IMAGE_PATH(w_quit_image_path.begin(), w_quit_image_path.end());
	W_QUIT_IMAGE_PATH = wW_QUIT_IMAGE_PATH;

	LuaObject W_QUIT_IMAGE_MO_PATH_obj = luaPState->GetGlobal("W_QUIT_IMAGE_MO_PATH");
	string w_quit_image_mo_path = W_QUIT_IMAGE_MO_PATH_obj.GetString();
	wstring wW_QUIT_IMAGE_MO_PATH(w_quit_image_mo_path.begin(), w_quit_image_mo_path.end());
	W_QUIT_IMAGE_MO_PATH = wW_QUIT_IMAGE_MO_PATH;

	LuaObject W_LIVES_HEAD_PATH_obj = luaPState->GetGlobal("W_LIVES_HEAD_PATH");
	string w_lives_head_path = W_LIVES_HEAD_PATH_obj.GetString();
	wstring wW_LIVES_HEAD_PATH(w_lives_head_path.begin(), w_lives_head_path.end());
	W_LIVES_HEAD_PATH = wW_LIVES_HEAD_PATH;

	LuaObject W_SHURIKEN_ICON_PATH_obj = luaPState->GetGlobal("W_SHURIKEN_ICON_PATH");
	string w_shuriken_icon_path = W_SHURIKEN_ICON_PATH_obj.GetString();
	wstring wW_SHURIKEN_ICON_PATH(w_shuriken_icon_path.begin(), w_shuriken_icon_path.end());
	W_SHURIKEN_ICON_PATH = wW_SHURIKEN_ICON_PATH;

	LuaObject W_TREAT_ICON_PATH_obj = luaPState->GetGlobal("W_TREAT_ICON_PATH");
	string w_treat_icon_path = W_TREAT_ICON_PATH_obj.GetString();
	wstring wW_TREAT_ICON_PATH(w_treat_icon_path.begin(), w_treat_icon_path.end());
	W_TREAT_ICON_PATH = wW_TREAT_ICON_PATH;

	LuaObject SPRITE_TYPES_DIR_obj = luaPState->GetGlobal("SPRITE_TYPES_DIR");
	string w_sprite_types_path = SPRITE_TYPES_DIR_obj.GetString();
	wstring wW_SPRITE_TYPES_DIR(w_sprite_types_path.begin(), w_sprite_types_path.end());
	SPRITE_TYPES_DIR = wW_SPRITE_TYPES_DIR;

	LuaObject SPRITE_TYPES_LIST_obj = luaPState->GetGlobal("SPRITE_TYPES_LIST");
	string w_sprite_types_list = SPRITE_TYPES_LIST_obj.GetString();
	wstring wW_SPRITE_TYPES_LIST(w_sprite_types_list.begin(), w_sprite_types_list.end());
	SPRITE_TYPES_LIST = wW_SPRITE_TYPES_LIST;

	LuaObject W_DEBUG_FILE_obj = luaPState->GetGlobal("W_DEBUG_FILE");
	W_DEBUG_FILE = W_DEBUG_FILE_obj.GetString();

	W_LEVEL_1_PATH = W_LEVEL_1_DIR + W_LEVEL_1_NAME;

}

/*
	Game  - Constructor, this method begins the 
	construction and loading of all major game objects. 
	This method only needs to be called once, when the 
	application is first started.

	NOTE: This constructor will not initialize the custom
	game objects: graphics, input, os, & timer.
	These must be constructed separately and fed to this
	object using the init method. Doing it this way allows for
	more platform independence.
*/
Game::Game()
{
	//This is the earliest possible time to read this file
	readLUA("data/NinjaCatCastle.lua");

	// NOTE THAT graphics, input, os, & timer
	// ARE CUSTOM GAME OBJECTS. DEPENDING ON WHAT TECHNOLOGY 
	// IS TO BE USED THESE OBJECT SHOULD BE CONSTRUCTED
	// AND THEN FED TO THIS Game USING THE init METHOD
	gsm = new GameStateManager();
	gui = new GameGUI();
	text = new GameText();

	//Creates the fmod sound system
	FMOD::System_Create(&fmodSystem);
	//default buffers is 1024 and 4, but this didn't work on my system for some reason.
	//this does.
	fmodSystem->setDSPBufferSize(1024, 6);
	fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	
}

/*
	~Game - Desctructor, it recovers memory allocated for
	the game objects. If Game is deleted, the application
	is closing, so everything should be cleaned up. Note that
	this 
*/
Game::~Game() 
{
	// THESE WERE CREATED BY Game, AND SO SHOULD BE
	// DELETED BY GAME
	delete gsm;
	delete gui;
	delete text;

	// THE GAME AND PLATFORM STUFF SHOULD BE DELETED
	// BY WHOEVER CREATED THEM
}

/*
	initPlatformPlugins - This method is to be used to feed this Game
	the constructed, technology-specific, objects. 
*/
void Game::initPlatformPlugins(		GameGraphics *initGraphics,
									GameInput *initInput,
									GameOS *initOS,									
									GameTimer *initTimer)
{
	// INITIALIZE ALL OF THE GAME'S CUSTOM OBJECTS
	graphics = initGraphics;
	input = initInput;
	os = initOS;
	timer = initTimer;
}

/*
	reloadAllDevices - Windows applications must cooperate
	with other running applications, so when someone hits
	ALT-TAB and switches from a full-screen game, it might
	lose ownership of the graphics card. This method can
	be called when a full-screen application retains ownership
	of all necessary resources such that all necessary
	data (like textures, sound, music, etc.) can be reloaded.
*/
void Game::reloadAllDevices()
{
	graphics->reloadGraphics();

	// WE MIGHT ADD MORE LATER
}
	
/*
	runGameLoop - This is the game loop management method.
	It runs continuously while the game is active. Once per
	frame it instructs the major game objects to get
	user input, record user input, update the GUI state, 
	update the sprites' states using AI and input, perform
	collision detection and resolution (physics), render
	the screen, etc.

	This loop is timed such that everything is kept to a 
	consistent framerate, thus the game should run 
	consistently on all machines.
*/
void Game::runGameLoop()
{
	// FIRST PROFILE?
	bool firstTimeThroughLoop = true;

	// LET'S START THE TIMER FROM SCRATCH
	timer->resetTimer();

	// KEEP RENDERING UNTIL SOMEONE PULLS THE PLUG
	while(gsm->isAppActive())
	{
		// UPDATES THE FMOD SOUND SYSTEM
		fmodSystem->update();

		// MOVE ALONG WINDOWS MESSAGES, THIS ALLOWS
		// US TO GET USER INPUT
		os->processOSMessages();

		// GET USER INPUT AND UPDATE GAME, GUI, OR PLAYER
		// STATE OR WHATEVER IS NECESSARY
		input->processInput(this);

		// IT IS POSSIBLE THE USER REQUESTED A SHUTDOWN
		// OF THE APP, SO WE NEED TO CHECK AGAIN
		if (gsm->isAppActive())
		{
			// USE THE INPUT TO UPDATE THE GAME
			processGameData();

			// UPDATE MUSIC HANDLING
			processMusicLogic();

			// AND RENDER THE GAME
			graphics->renderGame(this);
		}
	}
}

/*
	processGameData - This method directs game logic to be
	executed or not, depending on the game state, it addition
	it triggers the building or render lists and game loop
	timing each frame.
*/
void Game::processGameData()
{
	// WE ONLY PERFORM GAME LOGIC IF THE GAME
	// IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		gsm->update(this);
	}
	else if (gsm->isGameLevelLoading())
	{
		gsm->goToGame();
	}

	// UPDATE TEXT
	TextGenerator *tg = text->getTextGenerator();
	tg->updateText(this);

	// BUILD THE RENDER LISTS
	graphics->fillRenderLists(this);

	// KEEP THE FRAME RATE CONSISTENT
	timer->timeGameLoop();
}

/*
	quitGame - When called, this method will force the unloading of all game data
	loaded for the current level and then return the user to the main menu.
*/
void Game::quitGame()
{
	// Reset some player properties that may have changed
	if (gsm->getSpriteManager()->getPlayer()->getLives() == 0){
		gsm->getSpriteManager()->getPlayer()->setLives(3);
	}
	while (gsm->getSpriteManager()->getPlayer()->getInvincibilityFrames()){
		gsm->getSpriteManager()->getPlayer()->decrementInvincibilityFrames();
	}

	// CLEAN UP ALL THE WORLD TEXTURES
	graphics->clearWorldTextures();
	gsm->getSpriteManager()->unloadSprites(this);
	gsm->getWorld()->unloadWorld();

	// WE'RE GOING BACK TO THE MAIN MENU
	gsm->goToMainMenu();
}

/*
	shutdown - This method starts the termination of the game application, 
	cascading shutdown proceedings through all the necessary classes, resulting
	in the release of all game resources.
*/
void Game::shutdown()
{
	// MAKE SURE THE GAME LOOP ENDS AND THAT THE GSM CLEANS
	// UP ALL THE MEMORY IT IS USING
	gsm->shutdown(this);

	// CLEAR OUT AND RELEASE THE GPU
	graphics->shutdown();

	// RELEASE GAME CONTROLLERS IF NECESSARY
	input->shutdown();

	// DESTROYS FMOD
	fmodSystem->release();

	// WE MAY SHUTDOWN OTHER THINGS HERE LIKE SOUND & MUSIC
	// RESOURCES IN THE FUTURE
	// AND KILL THE WINDOW
	os->shutdown();
}

/*
	startGame - When called, this method will load the first level of the
	game and start play. Note that one must make sure the currentLeve
*/
void Game::startGame()
{
	// AND UPDATE THE GAME STATE OF COURSE
	gsm->goToLoadLevel();	// NOTE THAT CURRENTLY THERE IS NO LEVEL FILE,
	
	// THAT'S ONE THING YOU'LL BE DOING
	if (currentLevelFileName == W_LEVEL_1_NAME){
		dataLoader->loadWorld(this, W_LEVEL_1_DIR, W_LEVEL_1_NAME);
	}
	else if (currentLevelFileName == W_LEVEL_2_NAME){
		dataLoader->loadWorld(this, W_LEVEL_2_DIR, W_LEVEL_2_NAME);
	}
	else if (currentLevelFileName == W_LEVEL_3_NAME){
		dataLoader->loadWorld(this, W_LEVEL_3_DIR, W_LEVEL_3_NAME);
	}
}

//ALL LOGIC REGARDING MUSIC QUEUEING GOES HERE
//This will 99.9%-100% of the time be used to define what song
//to play after an intro to a song ends
void Game::processMusicLogic(){
	if (musicEnabled){
		GameState gs = gsm->getCurrentGameState();
		if (gs == GS_MAIN_MENU || gs == GS_HELP_SCREEN || gs == GS_ABOUT_SCREEN || gs == GS_GAME_IN_PROGRESS){
			bool isMusicPlaying = false;
			introChannel->isPlaying(&isMusicPlaying);
			if (!isMusicPlaying){
				musicChannel->setPaused(false);
			}
			/*if (musicDelay){
				if (musicDelay == 1){
					musicChannel->setPaused(false);
				}
				musicDelay--;
			}*/
		}
	}
}

void Game::toggleMusic(){
	if (musicEnabled){
		musicEnabled = !musicEnabled;
		introChannel->stop();
		musicChannel->stop();
	}
	else{
		musicEnabled = !musicEnabled;
		GameState gs = gsm->getCurrentGameState();
		if (gs == GS_MAIN_MENU || gs == GS_HELP_SCREEN || gs == GS_ABOUT_SCREEN){
			introChannel = playSongIntro(MAIN_MENU_SONG_INTRO, introChannel);
			musicChannel = queueSong(MAIN_MENU_SONG, musicChannel, 190);
		}
		if (currentLevelFileName == W_LEVEL_1_NAME){
			introChannel = playSongIntro(LEVEL_1_SONG_INTRO, introChannel);
			musicChannel = queueSong(LEVEL_1_SONG, musicChannel, 413);
		}
		else if (currentLevelFileName == W_LEVEL_2_NAME){
			introChannel = playSongIntro(LEVEL_2_SONG_INTRO, introChannel);
			musicChannel = queueSong(LEVEL_2_SONG, musicChannel, 413);
		}
		else{
			introChannel = playSongIntro(LEVEL_3_SONG_INTRO, introChannel);
			musicChannel = queueSong(LEVEL_3_SONG, musicChannel, 625);
		}
	}
	
}

//Plays a song's intro in the music channel. This sets loop to off so we can just loop the main
//song over and over again without worry about the intro (the intro and the main song have to be
//separate sound files for this to work)
//Logic for playing the main song after the intro is handled by the method processMusicLogic() right above.
FMOD::Channel* Game::playSongIntro(const char* song, FMOD::Channel* songChannel){
	if (musicEnabled){
		songChannel->stop(); //stops song currently playing in the music channel

		FMOD::Sound* newSong = 0;
		FMOD::Channel* newChannel = 0;

		fmodSystem->createSound(song, FMOD_DEFAULT, 0, &newSong);
		newSong->setMode(FMOD_LOOP_OFF); //doesn't loop

		fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel
		return newChannel; //returns newChannel
	}
	return songChannel;
}

//Queues a song for play to produce a (hopefully) gapless transition
//This also simplifies the music logic so it automatically plays the queued song
//as soon as the intro is done playing
FMOD::Channel* Game::queueSong(const char* song, FMOD::Channel* songChannel, int delay){
	if (musicEnabled){
		songChannel->stop(); //stops song currently playing in the music channel

		FMOD::Sound* newSong = 0;
		FMOD::Channel* newChannel = 0;

		fmodSystem->createStream(song, FMOD_DEFAULT, 0, &newSong);
		newSong->setMode(FMOD_LOOP_NORMAL); //loops

		fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel
		newChannel->setPaused(true);

		musicDelay = delay;

		return newChannel; //returns newChannel
	}
	return songChannel;
}

FMOD::Channel* Game::playOverlappingJingle(const char* song, FMOD::Channel* songChannel, int delay){
	if (musicEnabled){
		songChannel->stop(); //stops song currently playing in the music channel

		FMOD::Sound* newSong = 0;
		FMOD::Channel* newChannel = 0;

		fmodSystem->createStream(song, FMOD_DEFAULT, 0, &newSong);
		newSong->setMode(FMOD_LOOP_OFF); //doesn't loop

		fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel

		musicDelay = delay;

		return newChannel; //returns newChannel
	}
	return songChannel;
}

//Plays a song in the specified channel that doesn't have an intro.
FMOD::Channel* Game::playSongNoIntro(const char* song, FMOD::Channel* songChannel){
	if (musicEnabled){
		musicChannel->stop(); //stops song currently playing in the music channel
		introChannel->stop();

		FMOD::Sound* newSong = 0;
		FMOD::Channel* newChannel = 0;

		fmodSystem->createStream(song, FMOD_DEFAULT, 0, &newSong);
		newSong->setMode(FMOD_LOOP_NORMAL); //loops

		fmodSystem->playSound(newSong, 0, false, &newChannel); //plays sound in newChannel
		return newChannel; //returns newChannel
	}
	return songChannel;
}

//Plays a sound in first available channel
//Doesn't return the sound channel because we actually don't care about managing it.
//The channel gets freed automatically as soon as the sound ends.
void Game::playSound(const char* sound){
	FMOD::Sound* newSound = 0;
	FMOD::Channel* newChannel = 0;

	fmodSystem->createSound(sound, FMOD_DEFAULT, 0, &newSound);
	newChannel->setMode(FMOD_LOOP_OFF); //doesn't loop, will automatically be destoryed when sound ends

	fmodSystem->playSound(newSound, 0, false, &newChannel);
}