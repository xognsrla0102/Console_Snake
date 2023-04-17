#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "ingame_scene.h"
#include "scene_manager.h"
#include "sound_manager.h"
#include "utility.h"

typedef enum { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_STAY, DIR_NUMS } direction_t;

#define MAX_BODY_COUNT (5)
#define MAP_HEIGHT (35)
#define MAP_WIDTH (45)

#define MAP_START_POS_Y (6)
#define MAP_START_POS_X (10)

typedef struct {
	direction_t next_dir;
	direction_t dir;
	unsigned int body_count;
	COORD body_pos[MAX_BODY_COUNT];
	unsigned int my_score;
	DWORD last_move_time;
	DWORD move_interval;
	DWORD survival_time;
} snake_t;

static snake_t snake;
static const COORD MOVE_DIR[DIR_NUMS] = { {-2, 0}, {0, -1}, {2, 0}, {0, 1} };
static const int  MOVE_INTERVALS[10] = { 250, 200, 100, 80, 75, 60, 55, 50, 45, 40 };

static BOOL is_paused;
static BOOL is_game_over;

static COORD item_pos;

static const wchar_t maps[5][MAP_HEIGHT][MAP_WIDTH] =
{
	{
		L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¡¡', L'¢Ã',
		L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã', L'¢Ã',
	},
	{
		'¢Ã'
	},
	{
		'¢Ã'
	},
	{
		'¢Ã'
	},
	{
		'¢Ã'
	},
};

void spawn_item(void);

void init_ingame_scene(void)
{
	size_t h, w;
	for (h = 0; h < HEIGHT; ++h)
	{
		for (w = 0; w < WIDTH; ++w)
		{
			back_buffer[h][w] = L'¡¡';
		}
	}

	for (h = 0; h < MAP_HEIGHT; ++h)
	{
		for (w = 0; w < MAP_WIDTH; ++w)
		{
			back_buffer[h + MAP_START_POS_Y][w * 2 + MAP_START_POS_X] = maps[0][h][w];
		}
	}

	snake.body_count = 1;
	snake.dir = DIR_STAY;
	snake.next_dir = DIR_STAY;

	snake.body_pos[0].X = WIDTH / 2;
	snake.body_pos[0].Y = HEIGHT / 2;

	snake.my_score = 0u;

	snake.last_move_time = timeGetTime();

	snake.move_interval = MOVE_INTERVALS[0];

	snake.survival_time = timeGetTime();

	is_paused = FALSE;
	is_game_over = FALSE;

	spawn_item();
}

void release_ingame_scene(void)
{
}

static BOOL check_pause(void)
{
	if (input_key_delay(VK_ESCAPE))
	{
		system("cls");
		play_sfx(SFX_START);
		is_paused = !is_paused;

		if (is_paused == FALSE)
		{
			size_t h, w;
			for (h = 0; h < MAP_HEIGHT; ++h)
			{
				for (w = 0; w < MAP_WIDTH; ++w)
				{
					front_buffer[h + MAP_START_POS_Y][w * 2 + MAP_START_POS_X] = L'a';
				}
			}
		}
	}

	if (is_paused)
	{
		set_cursor_pos(20, 50);
		wprintf(L"ESC : °ÔÀÓÀ¸·Î µ¹¾Æ°¡±â");
		set_cursor_pos(21, 50);
		wprintf(L"¿£ÅÍ Å° : Å¸ÀÌÆ²·Î µ¹¾Æ°¡±â");

		if (input_key_delay(VK_RETURN))
		{
			play_sfx(SFX_START);
			change_scene(SCENE_MAIN, 1500);
		}
	}

	return is_paused;
}

static void move_snake(void)
{
	if (input_key(VK_LEFT) && (snake.body_count == 1 || snake.next_dir != DIR_RIGHT))
	{
		snake.next_dir = DIR_LEFT;
	}
	else if (input_key(VK_UP) && (snake.body_count == 1 || snake.next_dir != DIR_DOWN))
	{
		snake.next_dir = DIR_UP;
	}
	else if (input_key(VK_RIGHT) && (snake.body_count == 1 || snake.next_dir != DIR_LEFT))
	{
		snake.next_dir = DIR_RIGHT;
	}
	else if (input_key(VK_DOWN) && (snake.body_count == 1 || snake.next_dir != DIR_UP))
	{
		snake.next_dir = DIR_DOWN;
	}
}

static void spawn_item(void)
{
	while (TRUE)
	{
		int rand_y = rand() % MAP_HEIGHT;
		int rand_x = rand() % MAP_WIDTH;

		if (rand_x % 2 != 0)
		{
			continue;
		}

		if (back_buffer[MAP_START_POS_Y + rand_y][MAP_START_POS_X + rand_x] != L'¡¡')
		{
			continue;
		}

		size_t i;
		BOOL is_continue = FALSE;

		for (i = 0; i < snake.body_count; ++i)
		{
			if (snake.body_pos[i].Y == MAP_START_POS_Y + rand_y &&
				snake.body_pos[i].X == MAP_START_POS_X + rand_x)
			{
				is_continue = TRUE;
			}
		}

		if (is_continue)
		{
			continue;
		}

		back_buffer[MAP_START_POS_Y + rand_y][MAP_START_POS_X + rand_x] = L'¡Ú';
		break;
	}
}

static void game_over(void)
{
	stop_bgm();
	play_sfx(SFX_COLLIDE);

	print_color_text(L"¡á", snake.body_pos[0].Y, snake.body_pos[0].X, COLOR_RED);
	Sleep(1000);

	play_bgm(BGM_GAME_OVER, FALSE);

	size_t i;
	for (i = 1; i < snake.body_count; ++i)
	{
		if (!(snake.body_pos[0].X == snake.body_pos[i].X &&
			snake.body_pos[0].Y == snake.body_pos[i].Y))
		{
			print_color_text(L"¡Ø", snake.body_pos[i].Y, snake.body_pos[i].X, COLOR_RED);
		}
		Sleep(100);
	}

	Sleep(1000);
	change_scene(SCENE_MAIN, 1500);
	// change_scene(SCENE_RANKING, 1500);
}

static void check_collision(void)
{
	wchar_t block_info = back_buffer[snake.body_pos[0].Y][snake.body_pos[0].X];

	size_t i;
	for (i = 1; i < snake.body_count; ++i)
	{
		if (snake.body_pos[0].X == snake.body_pos[i].X &&
			snake.body_pos[0].Y == snake.body_pos[i].Y)
		{
			is_game_over = TRUE;
			return;
		}
	}

	switch (block_info)
	{
	case L'¢Ã':
		is_game_over = TRUE;
		break;
	case L'¡Ú':
		play_sfx(SFX_GET_ITEM);

		if (snake.body_count < MAX_BODY_COUNT)
		{
			snake.body_pos[snake.body_count].X = snake.body_pos[snake.body_count - 1].X - MOVE_DIR[snake.dir].X;
			snake.body_pos[snake.body_count].Y = snake.body_pos[snake.body_count - 1].Y - MOVE_DIR[snake.dir].Y;
			++snake.body_count;

			if (snake.body_count < 10)
			{
				snake.move_interval = MOVE_INTERVALS[snake.body_count - 1];
			}
			
			if (snake.body_count == MAX_BODY_COUNT)
			{
				play_sfx(SFX_POWER_UP);
				snake.my_score += 1000;
			}
		}

		snake.my_score += 100;
		spawn_item();
		break;
	case L'¡¡': break;
	default: assert(FALSE); break;
	}
}

void update_ingame_scene(void)
{
	if (check_pause())
	{
		return;
	}

	if (is_game_over)
	{
		game_over();
		return;
	}

	set_cursor_pos(1, 4);
	wprintf(L"ESCÅ°¸¦ ´©¸£¸é ÀÏ½Ã ÁßÁöÇÒ ¼ö ÀÖ½À´Ï´Ù.");

	wchar_t text[256];
	set_cursor_pos(2, 4);
	wsprintfW(text, L"ÇöÀç Á¡¼ö : %d point", snake.my_score);
	wprintf(text);

	set_cursor_pos(3, 4);
	wprintf(L"»ýÁ¸ ½Ã°£ : ");
	set_cursor_pos(3, 16);
	swprintf(text, 256, L"%.3f seconds", (timeGetTime() - snake.survival_time) / 1000.0);
	wprintf(text);

	move_snake();

	// Æ¯Á¤ ½Ã°£ °£°ÝÀ¸·Î ºí·Ï ¿òÁ÷ÀÓ °»½Å
	if (timeGetTime() - snake.last_move_time > snake.move_interval)
	{
		snake.dir = snake.next_dir;

		snake.last_move_time = timeGetTime();

		++snake.my_score;

		// ±âÁ¸ ÇÃ·¹ÀÌ¾îÀÇ À§Ä¡¸¦ °»½ÅÇÏ±â Àü¿¡ ¹Ì¸® ºó °ø°£À¸·Î º¯°æ
		int i;
		for (i = 0; (size_t)i < snake.body_count; ++i)
		{
			back_buffer[snake.body_pos[i].Y][snake.body_pos[i].X] = L'¡¡';
		}

		// ²¿¸® ºí·Ïµé ¹Ì¸® ÇÑ Ä­¾¿ ÀÌµ¿½ÃÅ´
		assert(snake.body_count > 0);
		for (i = snake.body_count - 1; i > 0; --i)
		{
			snake.body_pos[i].X = snake.body_pos[i - 1].X;
			snake.body_pos[i].Y = snake.body_pos[i - 1].Y;
		}

		// Çìµå ºí·Ï ¿òÁ÷ÀÓ
		snake.body_pos[0].X += MOVE_DIR[snake.dir].X;
		snake.body_pos[0].Y += MOVE_DIR[snake.dir].Y;

		check_collision();

		// »õ·Ó°Ô °»½ÅµÈ Áö··ÀÌ ºí·°À» ¹é ¹öÆÛ¿¡ ¸¸µé±â
		for (i = 0; (size_t)i < snake.body_count; ++i)
		{
			back_buffer[snake.body_pos[i].Y][snake.body_pos[i].X] = L'¡á';
		}
	}
}
