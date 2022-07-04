#ifndef INPUT_SYS_H
#define INPUT_SYS_H

#include <stdint.h>
#include <stdbool.h>

enum KEY {
	KEY_NONE	= 0x1,
	KEY_W		= 0x00000001,
	KEY_A		= 0x00000002,
	KEY_S		= 0x00000004,
	KEY_D		= 0x00000008,
	KEY_SPACE	= 0x00000010,
	KEY_LCTRL	= 0x00000020,
	KEY_SHIFT	= 0x00000040,
	KEY_ESC		= 0x00000080,
	KEY_F1		= 0x00000100
};

// get input from glfw window and store it in input buffer
void input_update_buffer();

// clear the input buffer
void input_clear_buffer();

// check if a key is pressed
bool input_key_pressed(enum KEY);

#ifdef XEN_DEBUG
// debug/testing utility
void input_set_buffer(uint8_t);
#endif // XEN_DEBUG

#endif //  INPUT_SYS_H
