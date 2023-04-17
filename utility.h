#pragma once

#include <Windows.h>

typedef enum {
	COLOR_RED = 12,
	COLOR_YELLOW = 14,
	// COLOR_YELLOW = 6,
} text_color_type_t;

void set_console(void);
void set_cursor_visible(const BOOL visible);
void set_cursor_pos(const int y, const int x);
void print_color_text(const wchar_t* text, const int y, const int x, const text_color_type_t color);

BOOL input_key(const int key_code);
BOOL input_key_delay(const int key_code);
