#pragma once

typedef enum {
	BGM_NONE,
	BGM_MAIN,
	BGM_GAME_OVER,
	BGM_NUMS,
} bgm_type_t;

typedef enum {
	SFX_START,
	SFX_SELECT,
	SFX_GET_ITEM,
	SFX_COLLIDE,
	SFX_POWER_UP,
	SFX_NUMS,
} sfx_type_t;

void init_sound_manager(void);
void release_sound_manager(void);

void play_bgm(const bgm_type_t bgm_type, const BOOL is_repeat);
void stop_bgm(void);

void play_sfx(const sfx_type_t sfx_type);
