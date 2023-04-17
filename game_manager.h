#pragma once

#include <Windows.h>

#define WIDTH (140)
#define HEIGHT (45)

extern BOOL exit_game;

void init_game_manager(void);
void release_game_manager(void);

void update_game_manager(void);
void render_game_manager(void);
