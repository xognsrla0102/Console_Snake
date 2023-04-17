#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#include "game_manager.h"
#include "main_scene.h"
#include "scene_manager.h"
#include "sound_manager.h"
#include "utility.h"

#define WORD_COUNT (5)
#define WORD_WIDTH (10)
#define WORD_HEIGHT (10)

typedef enum {
	MENU_START,
	MENU_RANKING,
	MENU_CREATOR,
	MENU_EXIT,
	MENU_NUMS
} menu_t;

static wchar_t title_word[WORD_COUNT][WORD_HEIGHT][WORD_WIDTH] =
{
	{
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
	},
	{
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'■', L'■', L'■', L'■', L'　', L'■', L'■',
		L'■', L'■', L'　', L'■', L'■', L'■', L'■', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
	},
	{
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'■', L'■',
	},
	{
		L'　', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'　',
		L'　', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'　',
		L'　', L'■', L'■', L'　', L'　', L'■', L'■', L'　', L'　', L'　',
		L'　', L'■', L'■', L'　', L'　', L'■', L'■', L'　', L'　', L'　',
		L'　', L'■', L'■', L'■', L'■', L'　', L'　', L'　', L'　', L'　',
		L'　', L'■', L'■', L'■', L'■', L'　', L'　', L'　', L'　', L'　',
		L'　', L'■', L'■', L'　', L'　', L'■', L'■', L'　', L'　', L'　',
		L'　', L'■', L'■', L'　', L'　', L'■', L'■', L'　', L'　', L'　',
		L'　', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'　',
		L'　', L'■', L'■', L'　', L'　', L'　', L'　', L'■', L'■', L'　',
	},
	{
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'　', L'　', L'　', L'　', L'　', L'　', L'　', L'　',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
		L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■', L'■',
	},
};

static BOOL show_game_menu;
static BOOL show_ranking;
static BOOL show_creator;

static BOOL is_blink;
static DWORD blink_start_game_text_flag_time;
static unsigned int selected_menu_num;

static const DWORD KEY_INPUT_INTERVAL = 100;
static DWORD start_key_input_time;

void init_main_scene(void)
{
	play_bgm(BGM_MAIN, TRUE);

	size_t count, height, width;
	for (count = 0; count < WORD_COUNT; ++count)
	{
		for (height = 0; height < WORD_HEIGHT; ++height)
		{
			set_cursor_pos(8 + height, 10 + count * 24);
			for (width = 0; width < WORD_WIDTH; ++width)
			{
				wprintf(L"%c", title_word[count][height][width]);
			}
			wprintf(L"\n");
		}
		Sleep(200);
	}

	show_game_menu = FALSE;
	show_ranking = FALSE;
	show_creator = FALSE;

	is_blink = FALSE;
	blink_start_game_text_flag_time = timeGetTime();
	selected_menu_num = 0u;

	set_cursor_pos(30, 55);
	wprintf(L"시작하려면 엔터키를 누르세요.");
}

void release_main_scene(void)
{
	return;
}

void update_main_scene(void)
{
	if (show_game_menu == FALSE)
	{
		if (input_key_delay(VK_RETURN))
		{
			play_sfx(SFX_START);
			show_game_menu = TRUE;
			set_cursor_pos(30, 55);
			wprintf(L"                                     ");
			return;
		}

		if (timeGetTime() - blink_start_game_text_flag_time > 500)
		{
			blink_start_game_text_flag_time = timeGetTime();

			is_blink = !is_blink;

			if (is_blink)
			{
				set_cursor_pos(30, 55);
				wprintf(L"                                     ");
			}
			else
			{
				set_cursor_pos(30, 55);
				wprintf(L"시작하려면 엔터키를 누르세요.");
			}
		}
	}
	else
	{
		const unsigned int start_menu_text_height = 27;

		if (show_ranking)
		{
			set_cursor_pos(2, 4);
			wprintf(L"메뉴로 돌아가시려면 ESC키를 눌러주세요.");

			set_cursor_pos(7, 30);
			wprintf(L"========================== [ 랭킹 ] ==========================");
			set_cursor_pos(38, 30);
			wprintf(L"=============================================================");

			if (input_key_delay(VK_ESCAPE))
			{
				change_scene(SCENE_MAIN, 500);
				return;
			}
		}
		else if (show_creator)
		{
			set_cursor_pos(2, 4);
			wprintf(L"메뉴로 돌아가시려면 ESC키를 눌러주세요.");

			set_cursor_pos(20, 60);
			wprintf(L"개발자 : 김태훈");

			if (input_key_delay(VK_ESCAPE))
			{
				change_scene(SCENE_MAIN, 500);
				return;
			}
		}
		else
		{
			set_cursor_pos(23, 35);
			wprintf(L"======================= [ 게임 메뉴 ] ========================");
			set_cursor_pos(start_menu_text_height, 60);
			wprintf(L"게임 시작");
			set_cursor_pos(start_menu_text_height + 3, 60);
			wprintf(L"랭킹 확인");
			set_cursor_pos(start_menu_text_height + 6, 60);
			wprintf(L"제작자");
			set_cursor_pos(start_menu_text_height + 9, 60);
			wprintf(L"게임 종료");
			set_cursor_pos(41, 35);
			wprintf(L"===============================================================");

			if (input_key_delay(VK_UP) &&
				timeGetTime() - start_key_input_time > KEY_INPUT_INTERVAL)
			{
				play_sfx(SFX_SELECT);

				start_key_input_time = timeGetTime();

				set_cursor_pos(start_menu_text_height + 3 * selected_menu_num, 55);
				wprintf(L"  ");
				
				selected_menu_num = selected_menu_num > 0 ? selected_menu_num - 1 : 3;
			}
			else if (input_key_delay(VK_DOWN) &&
				timeGetTime() - start_key_input_time > KEY_INPUT_INTERVAL)
			{
				play_sfx(SFX_SELECT);

				start_key_input_time = timeGetTime();

				set_cursor_pos(start_menu_text_height + 3 * selected_menu_num, 55);
				wprintf(L"  ");

				selected_menu_num = selected_menu_num < 3 ? selected_menu_num + 1 : 0;
			}

			set_cursor_pos(start_menu_text_height + 3 * selected_menu_num, 55);
			wprintf(L"○");

			if (input_key_delay(VK_RETURN))
			{
				play_sfx(SFX_START);

				system("cls");

				switch (selected_menu_num)
				{
				case MENU_START: change_scene(SCENE_INGAME, 1500); break;
				case MENU_RANKING: show_ranking = TRUE; break;
				case MENU_CREATOR: show_creator = TRUE; break;
				case MENU_EXIT: exit_game = TRUE; break;
				default: assert(FALSE); break;
				}
			}
		}
	}
}
