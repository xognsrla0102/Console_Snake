#include <assert.h>
#include <stdio.h>
#include <Windows.h>

#include "game_manager.h"
#include "loading_scene.h"
#include "scene_manager.h"
#include "utility.h"

#define LOADING_BLOCK_COUNT (10)

unsigned int loading_milisec = 0u;
scene_type_t import_scene = SCENE_NONE;

static COORD pos[LOADING_BLOCK_COUNT];
static DWORD start_loading_time;

typedef enum { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_NUMS } direction_t;

static direction_t dir;

void init_loading_scene(void)
{
	size_t i;
	for (i = 0; i < LOADING_BLOCK_COUNT; ++i)
	{
		pos[i].X = WIDTH / 2;
		pos[i].Y = HEIGHT / 2 - 8;
	}

	size_t h, w;
	for (h = 0; h < HEIGHT; ++h)
	{
		for (w = 0; w < WIDTH; ++w)
		{
			back_buffer[h][w] = L'　';
		}
	}

	start_loading_time = timeGetTime();

	dir = DIR_RIGHT;

	set_cursor_pos(HEIGHT / 2, WIDTH / 2 - 4);
	wprintf(L"로딩 중...");
}

void release_loading_scene(void)
{	
}

void update_loading_scene(void)
{
	const COORD MOVE_DIR[DIR_NUMS] = { {-2, 0}, {0, -1}, {2, 0}, {0, 1} };
	const COORD END_POS[DIR_NUMS] = { {58, 20}, {58, 14}, {80, 14}, {80, 20} };	

	if (timeGetTime() - start_loading_time > loading_milisec)
	{
		break_loading_scene();
		return;
	}

	// 기존 플레이어의 위치를 갱신하기 전에 미리 빈 공간으로 변경
	size_t i;
	for (i = 0; i < LOADING_BLOCK_COUNT; ++i)
	{
		back_buffer[pos[i].Y][pos[i].X] = L'　';
	}

	// 헤드 블록이 움직이기 전에, 미리 꼬리 블록들을 한 칸씩 이동시킴
	for (i = LOADING_BLOCK_COUNT - 1; i > 0; --i)
	{
		pos[i].X = pos[i - 1].X;
		pos[i].Y = pos[i - 1].Y;
	}

	// 헤드 블록을 움직이고
	pos[0].X += MOVE_DIR[dir].X;
	pos[0].Y += MOVE_DIR[dir].Y;

	if (pos[0].X == END_POS[dir].X && pos[0].Y == END_POS[dir].Y)
	{
		dir = (dir + 1) % DIR_NUMS;
	}

	// 새롭게 갱신된 지렁이 블럭을 백 버퍼에 만들기
	for (i = 0; i < LOADING_BLOCK_COUNT; ++i)
	{
		back_buffer[pos[i].Y][pos[i].X] = L'■';
	}

	Sleep(50);
}
