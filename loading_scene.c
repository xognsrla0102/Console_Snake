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
			back_buffer[h][w] = L'��';
		}
	}

	start_loading_time = timeGetTime();

	dir = DIR_RIGHT;

	set_cursor_pos(HEIGHT / 2, WIDTH / 2 - 4);
	wprintf(L"�ε� ��...");
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

	// ���� �÷��̾��� ��ġ�� �����ϱ� ���� �̸� �� �������� ����
	size_t i;
	for (i = 0; i < LOADING_BLOCK_COUNT; ++i)
	{
		back_buffer[pos[i].Y][pos[i].X] = L'��';
	}

	// ��� ����� �����̱� ����, �̸� ���� ��ϵ��� �� ĭ�� �̵���Ŵ
	for (i = LOADING_BLOCK_COUNT - 1; i > 0; --i)
	{
		pos[i].X = pos[i - 1].X;
		pos[i].Y = pos[i - 1].Y;
	}

	// ��� ����� �����̰�
	pos[0].X += MOVE_DIR[dir].X;
	pos[0].Y += MOVE_DIR[dir].Y;

	if (pos[0].X == END_POS[dir].X && pos[0].Y == END_POS[dir].Y)
	{
		dir = (dir + 1) % DIR_NUMS;
	}

	// ���Ӱ� ���ŵ� ������ ���� �� ���ۿ� �����
	for (i = 0; i < LOADING_BLOCK_COUNT; ++i)
	{
		back_buffer[pos[i].Y][pos[i].X] = L'��';
	}

	Sleep(50);
}
