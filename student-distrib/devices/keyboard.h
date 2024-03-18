/* keyboard.h - Defines used in interactions with the keyboard 
* 
*
*/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "../types.h"
#include "../i8259.h"

#define Keyboard_port 0x60

uint16_t keycodes[0xD7];


/* init functiuon for keybaord */
void keyboard_init(void);

uint16_t get_key(void);

#endif