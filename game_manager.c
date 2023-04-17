#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>

#include "game_manager.h"
#include "sound_manager.h"
#include "scene_manager.h"
#include "utility.h"

BOOL exit_game = FALSE;

void init_game_manager(void)
{
	setlocale(LC_ALL, ".utf8");

	srand((unsigned int)time(NULL));

	set_console();

	init_sound_manager();
	init_scene_manager();
}

void release_game_manager(void)
{
	release_sound_manager();
	set_cursor_pos(0, 0);
}

void update_game_manager(void)
{
	update_scene_manager();
}

void render_game_manager(void)
{
	render_scene_manager();
}
