#pragma once

#include <Windows.h>

#include "scene_manager.h"

extern unsigned int loading_milisec;
extern scene_type_t import_scene;

void init_loading_scene(void);
void release_loading_scene(void);
void update_loading_scene(void);
