#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <Windows.h>

#include "game_manager.h"
#include "utility.h"

static HANDLE console_output_handle = NULL;

void set_console(void)
{
	char console_command[256];
	sprintf(console_command, "mode con:cols=%d lines=%d", WIDTH, HEIGHT);
	system(console_command);

	SetConsoleTitleW(L"\'간지\'나는 스네이크 게임");

	console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void set_cursor_visible(const BOOL visible)
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, visible };
	SetConsoleCursorInfo(console_output_handle, &cursor_info);
}

void set_cursor_pos(const int y, const int x)
{
	COORD pos = { (SHORT)x, (SHORT)y };
	BOOL is_success = SetConsoleCursorPosition(console_output_handle, pos);
	assert(is_success != FALSE);
}

void print_color_text(const wchar_t* text, const int y, const int x, const text_color_type_t color)
{
	SetConsoleTextAttribute(console_output_handle, color);
	set_cursor_pos(y, x);
	wprintf(text);

	SetConsoleTextAttribute(console_output_handle, 7);
}

BOOL input_key(const int key_code)
{
	return GetAsyncKeyState(key_code) & 0x8000;
}

BOOL input_key_delay(const int key_code)
{
	return GetAsyncKeyState(key_code) & 0x0001;
}
