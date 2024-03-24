#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
/* constants to be used within terminal */
//values taken from discussion slides
#define TERMINAL_COLUMNS 80
#define TERMINAL_ROWS 25
#define KEYBOARD_BUFFER_SIZE 128

int screen_x;
int screen_y;

void clear_terminal();
int terminal_open(const uint8_t* filename);
int terminal_close();
int terminal_read();
int terminal_write();

#endif

