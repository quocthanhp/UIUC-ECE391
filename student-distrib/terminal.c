#include "terminal.h"
#include "lib.h"
#include "i8259.h"
#include "devices/keyboard.h"



/*
 * terminal clear 
 * clears the terminal's screen and sets x and y vectors to point to the top left of the screen
 * uses clear() defined in lib.h
 * INPUTS: NONE
 * OUTPUTS: NONE
 * SIDE EFFECTS: clears 
 */

void clear_terminal(void){

    //set the contents of video memory to be blank
    //clear() defined within lib.h
    clear();

    //reset the cursor to the top left postion. 
    screen_x = 0;
    screen_y = 0;
}

/*
 * terminal open
 * sets default values used and calls clear terminal
 * INPUTS: const uint8_t * filename (pointer to a file name)
 * OUTPUTS: returns 0 on success
 */
int terminal_open(const uint8_t * filename){
    
    clear_terminal();
    return 0;
}

/*
 * terminal read
 * reads from keyboard buffer
 * loads keyboard buffer value into a terminal buffer
 * INPUTS:
 * OUTPUTS: number of bytes read
 * SIDE EFFECTS: none
 */

int terminal_read(){
    return 0;
}

/*
 * terminal write
 * writes to the screen from the terminal buffer
 * loads values to be output onto video memory
 * INPUTS:
 * OUTPUTS: number of bytes written, -1 if no bytes were written
 * SIDE EFFECTS: none
 */

int terminal_write(){
    return 0;
}