/* keyboard.h - Defines used in interactions with the keyboard 
* 
*
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "../types.h"
#include "../i8259.h"

#define Keyboard_port 0x60

// uint16_t keycodes[0xD7];
// uint8_t scan_code;


/* init functiuon for keybaord */
void keyboard_init(void);
void keyboard_interrupt(void);
void print_code(uint8_t scan_code);
uint8_t get_key(void);

#endif
