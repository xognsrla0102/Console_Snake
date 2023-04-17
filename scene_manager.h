#pragma once

#include "game_manager.h"

typedef enum scene_type {
	SCENE_NONE,
	SCENE_LOADING,
	SCENE_MAIN,
	SCENE_INGAME,
	SCENE_RANKING,
	SCENE_NUMS
} scene_type_t;

extern wchar_t front_buffer[HEIGHT][WIDTH];
extern wchar_t back_buffer[HEIGHT][WIDTH];

void init_scene_manager(void);

void change_scene(const scene_type_t scene_type, const unsigned int _loading_milisec);
void break_loading_scene(void);

void update_scene_manager(void);
void render_scene_manager(void);
