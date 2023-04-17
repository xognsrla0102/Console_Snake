#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <Windows.h>

#include "scene_manager.h"

#include "game_manager.h"
#include "loading_scene.h"
#include "main_scene.h"
#include "ingame_scene.h"
#include "ranking_scene.h"
#include "utility.h"

static scene_type_t now_scene_state;

static void(*scene_init_func_ptr[SCENE_NUMS])(void);
static void(*scene_release_func_ptr[SCENE_NUMS])(void);
static void(*scene_update_func_ptr[SCENE_NUMS])(void);

wchar_t front_buffer[HEIGHT][WIDTH];
wchar_t back_buffer[HEIGHT][WIDTH];

#define INIT_FUNC_PTR(scene_type, scene_name) \
scene_init_func_ptr[##scene_type##] = init_##scene_name##_scene; \
scene_release_func_ptr[##scene_type##] = release_##scene_name##_scene; \
scene_update_func_ptr[##scene_type##] = update_##scene_name##_scene;

void init_scene_manager(void)
{
	size_t h, w;
	for (h = 0; h < HEIGHT; ++h)
	{
		for (w = 0; w < WIDTH; ++w)
		{
			front_buffer[h][w] = back_buffer[h][w] = L'¡¡';
		}
	}

	set_cursor_visible(FALSE);

	INIT_FUNC_PTR(SCENE_LOADING, loading);
	INIT_FUNC_PTR(SCENE_MAIN, main);
	INIT_FUNC_PTR(SCENE_INGAME, ingame);
	INIT_FUNC_PTR(SCENE_RANKING, ranking);

	change_scene(SCENE_MAIN, 4000);
}

void change_scene(const scene_type_t scene_type, const unsigned int _loading_milisec)
{
	system("cls");

	if (now_scene_state != SCENE_NONE)
	{
		scene_release_func_ptr[now_scene_state]();
	}

	now_scene_state = SCENE_LOADING;
	loading_milisec = _loading_milisec;
	import_scene = scene_type;
	scene_init_func_ptr[now_scene_state]();
}

void break_loading_scene(void)
{
	system("cls");

	scene_release_func_ptr[now_scene_state]();
	now_scene_state = import_scene;
	import_scene = SCENE_NONE;
	scene_init_func_ptr[now_scene_state]();
}

void update_scene_manager(void)
{
	scene_update_func_ptr[now_scene_state]();
}

void render_scene_manager(void)
{
	size_t h, w;
	for (h = 0; h < HEIGHT; ++h)
	{
		for (w = 0; w < WIDTH; ++w)
		{
			if (front_buffer[h][w] != back_buffer[h][w])
			{
				front_buffer[h][w] = back_buffer[h][w];
				set_cursor_pos(h, w);
				if (front_buffer[h][w] == L'¡Ú')
				{
					print_color_text(L"¡Ú", h, w, COLOR_YELLOW);
				}
				else
				{
					wprintf(L"%c", front_buffer[h][w]);
				}
			}
		}
	}
}
