#include "terminal.h"
#include "lib.h"
#include "i8259.h"
#include "devices/keyboard.h"


terminal terminal_;

/*
 * terminal clear 
 * clears the terminal's screen and sets x and y vectors to point to the top left of the screen
 * uses clear() defined in lib.h
 * INPUTS: NONE
 * OUTPUTS: NONE
 * SIDE EFFECTS: clears 
 */

void clear_terminal(terminal terminal_){

    //set the contents of video memory to be blank
    //clear() defined within lib.h
    clear();

    //reset the cursor to the top left postion. 
    // terminal_.screen_x = 0;
    // terminal_.screen_y = 0;

    /*need to add something to change cursor position to be in the top left */
}

/*
 * terminal open
 * sets default values used and calls clear terminal
 * INPUTS: const uint8_t * filename (pointer to a file name)
 * OUTPUTS: returns 0 on success
 */
int terminal_open(const uint8_t * filename){
    
    int i;
    terminal terminal_;

    terminal_.position = 0; // initializing the "actual" buffer size as 0 
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
            terminal_.terminal_buffer[i] = '\0';    //setting the static terminal buffer to be null characters
        }
    clear_terminal(terminal_);
    return 0;

}

/*
 * terminal read
 * reads from keyboard buffer
 * loads keyboard buffer value into a terminal buffer
 * INPUTS: fd, buf. nbytes
 * OUTPUTS: number of bytes read, -1 if no bytes were read.
 * SIDE EFFECTS: none
 */

int terminal_read(int32_t fd, void * buf, int32_t nbytes){

    int bytes_read = 0; //return value, will be updated within the function
    int32_t i;
    int32_t range;
    // int enter_flag = get_enter_flag();
    //check if the last character in the terminal buffer is '\n' (CA suggestion instead of using an enter flag)
    while ( (terminal_.terminal_buffer[terminal_.position]) != '\n' ){
        //do nothing 
    } 

    cli();

    //print as many bytes as possible and copy until \n

    // check if the number of bytes we want to read is less that the max size of the buffer
    //  update the actual range we want to use according to the conditon
    if(nbytes < KEYBOARD_BUFFER_SIZE){  
        range = nbytes;
    }
    else{
        range = KEYBOARD_BUFFER_SIZE;
    }

    for (i = 0; i < range; i++){

        if( terminal_.terminal_buffer[i] != '\n'){
            ((char*) buf)[i] = terminal_.terminal_buffer[i];
            bytes_read = bytes_read + 1;
        }
        //we don't want to read beyond the first '\n' character
        else{
            break;
        }

    }

    sti();
    if(bytes_read > 0){
        return bytes_read;
    }
    else{
        return -1;
    }
    //similar arguments to a file (user buf bytes blah blah blah)
    // while enter isn't pressed 
    //keyboard will send interrupts 
    //add stuff to the buffer
    //memcopy to read to the user buffer 

}

//call 
/*
 * terminal write
 * writes to the screen from the terminal buffer
 * loads values to be output onto video memory
 * INPUTS:
 * OUTPUTS: number of bytes written, -1 if no bytes were written
 * SIDE EFFECTS: none
 */

int terminal_write(int32_t fd, const void * buf, int32_t nbytes){

    //input buffer (sys call)
    //for loop and put c
    int32_t i;
    char current_character;

    //check the conditions with TAs
    for( i = 0; i < nbytes; i++){
        current_character = ((char *) buf)[i];
        putc(current_character);
    }
    return nbytes;
}

/*
 * terminal update buffer
 * updates the terminal-keyboard buffer
 * by adding a specific character generated from the keyboard interrupt
 * INPUTS: unsigned char character
 * OUTPUTS: none
 * SIDE EFFECTS: updates the terminal buffer
 */

 void terminal_update_buffer(unsigned char character) {

    if( (terminal_.position < 127) ){

        terminal_.position = (terminal_.position)++;
        terminal_.terminal_buffer[terminal_.position] = character;
    }
   // what should 
    
 }

 void terminal_remove_from_buffer(){
    if(terminal_.position > 0){
        int delete_pos = terminal_.position;
        terminal_.terminal_buffer[delete_pos] = ' ';
        terminal_.position = terminal_.position - 1;
    }
 }