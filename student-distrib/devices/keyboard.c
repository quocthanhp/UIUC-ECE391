#include "lib.h"
#include "keyboard.h"
#include "i8259.h"

/* straight from osDEV */
/* DESC : Init the keyboard
*  INPUT : void
*  OUTPUT : void 
*/
void keyboard_int(void){
    /* google says keybaord is usually irq 1*/
    enable_irq( 1 ) 
}