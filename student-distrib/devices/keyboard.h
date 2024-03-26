/* keyboard.h - Defines used in interactions with the keyboard 
* 
*
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "../types.h"
#include "../i8259.h"

#define Keyboard_port 0x60

#define BACKSPACE_PRESSED       0x0E
#define TAB_PRESSED             0x0F
#define ENTER_PRESSED           0x1C
#define LEFT_CONTROL_PRESSED    0x1D
#define LEFT_SHIFT_PRESSED      0x2A
#define RIGHT_SHIFT_PRESSED     0x36
#define LEFT_ALT_PRESSED        0x38
#define CAPS_LOCK_PRESSED       0x3A
// #define RIGHT_CONTROL_PRESSED   0xE0 

#define L_PRESSED               0x26
#define L_RELEASED              0xA6
#define BACKSPACE_RELEASED      0x8E
// #define TAB_RELEASED            0x8F
#define LEFT_CONTROL_RELEASED   0x9D
#define LEFT_SHIFT_RELEASED     0xAA
#define RIGHT_SHIFT_RELEASED    0xB6
#define ENTER_RELEASED          0x9C
// #define RIGHT_CONTROL_RELEASED  0xE0
//uint16_t keycodes[0xD7];
// uint8_t scan_code;

unsigned char lowercase_characters[128];
unsigned char uppercase_characters[128];
unsigned char shifted_characters[128];


char keyboard_buffer[128];

/* init functiuon for keybaord */
void keyboard_init(void);
void keyboard_interrupt(void);
void print_code(uint8_t scan_code);
uint8_t get_key(void);

int get_enter_flag();
#endif
