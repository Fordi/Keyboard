#ifndef KEYMAP_H
#define KEYMAP_H

#include <keylayouts.h>

#define MODIFIERKEY_FN 0x8f

#define MATRIX_ROWS 17

#define MATRIX_COLS 8

int normal[MATRIX_ROWS][MATRIX_COLS] = {
	{ KEYPAD_1, KEY_PRINTSCREEN, KEYPAD_SLASH, 0, KEYPAD_4, KEYPAD_7, KEYPAD_5, KEY_RIGHT },
	{ KEYPAD_0, KEY_DELETE, KEYPAD_PLUS, 0, KEYPAD_3, KEYPAD_9, KEYPAD_6, KEY_DOWN },
	{ KEY_UP, KEY_HOME, KEYPAD_PERIOD, KEY_END, 0, KEYPAD_ENTER, KEYPAD_MINUS, KEY_LEFT },
	{ 0, 0, 0, KEY_9, KEY_O, KEY_L, KEY_PERIOD, 0 },
	{ KEY_QUOTE, KEY_MINUS, KEY_LEFT_BRACE, KEY_0, KEY_P, KEY_SEMICOLON, 0, KEY_SLASH },
	{ KEY_F6, KEY_EQUAL, KEY_RIGHT_BRACE, KEY_8, KEY_I, KEY_K, KEY_COMMA, 0 },
	{ KEY_H, KEY_6, KEY_Y, KEY_7, KEY_U, KEY_J, KEY_M, KEY_N },
	{ KEY_F5, 0, KEY_BACKSPACE, 0, 0, KEY_BACKSLASH, KEY_ENTER, KEY_SPACE },
	{ KEY_G, KEY_5, KEY_T, KEY_4, KEY_R, KEY_F, KEY_V, KEY_B },
	{ KEY_F4, KEY_F2, KEY_F3, KEY_3, KEY_E, KEY_D, KEY_C, 0 },
	{ 0, 0, KEY_CAPS_LOCK, KEY_2, KEY_W, KEY_S, KEY_X, 0 },
	{ KEY_ESC, KEY_TILDE, KEY_TAB, KEY_1, KEY_Q, KEY_A, KEY_Z, KEY_MENU },
	{ 0, 0, 0, KEY_INSERT, KEY_NUM_LOCK, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, KEYPAD_8, 0, 0, KEY_PAGE_UP, KEY_PAGE_DOWN, KEYPAD_ASTERIX, KEYPAD_2 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }
};

int modifier[MATRIX_ROWS][MATRIX_COLS] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ MODIFIERKEY_LEFT_ALT, 0, 0, 0, 0, 0, 0, MODIFIERKEY_RIGHT_ALT },
	{ 0, 0, MODIFIERKEY_LEFT_SHIFT, 0, 0, 0, MODIFIERKEY_RIGHT_SHIFT, 0 },
	{ 0, MODIFIERKEY_LEFT_CTRL, 0, 0, 0, 0, MODIFIERKEY_RIGHT_CTRL, 0 },
	{ 0, 0, 0, MODIFIERKEY_GUI, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, MODIFIERKEY_FN, 0, 0 }
};

int fn_keys[MATRIX_ROWS][MATRIX_COLS] = {
	{ 0, 0, 0, KEY_MEDIA_NEXT_TRACK, 0, 0, 0, 0 },
	{ 0, 0, 0, KEY_MEDIA_PLAY_PAUSE, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, KEY_MEDIA_VOLUME_DEC, KEY_MEDIA_MUTE, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, KEY_MEDIA_VOLUME_INC, 0, KEY_MEDIA_PREV_TRACK, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, KEY_SYSTEM_SLEEP, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }
};

int Row_IO[MATRIX_ROWS] = { 21, 2, 20, 3, 19, 4, 18, 5, 17, 6, 24, 7, 25, 8, 33, 9, 26 };

int Col_IO[MATRIX_COLS] = { 10, 27, 11, 28, 12, 32, 31, 30 };

#endif
