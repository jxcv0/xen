#ifndef INPUT_SYS_H
#define INPUT_SYS_H

#include <stdint.h>
#include <stdbool.h>

// #define W_KEY	0x00000001
// #define A_KEY	0x00000002
// #define S_KEY	0x00000004
// #define D_KEY	0x00000008
// #define SPACE_KEY	0x00000010
// #define LCTRL_KEY	0x00000020
// #define SHIFT_KEY	0x00000040
// #define ESC_KEY	0x00000080
// #define F1_KEY	0x00000100

enum KEY {
	NONE	= 0x0,
	W	= 0x00000001,
	A	= 0x00000002,
	S	= 0x00000004,
	D	= 0x00000008,
	SPACE	= 0x00000010,
	LCTRL	= 0x00000020,
	SHIFT	= 0x00000040,
	ESC	= 0x00000080,
	F1	= 0x00000100
};

// get input from glfw window and store it in input buffer
void input_update_buffer();

// clear the input buffer
void input_clear_buffer();

// check if a key is pressed
bool input_key_pressed(enum KEY);

#endif //  INPUT_SYS_H
