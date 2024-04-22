#include "terminal.h"
#include "lib.h"
#include "i8259.h"
#include "devices/keyboard.h"
#include "nPage.h"
#include "syscall.h"

#define VIDEO       0xB8000
#define NUM_COLS    80
#define NUM_ROWS    25
#define ATTRIB      0x7

terminal_t terminals[3]; // make array of 3
int active_terminal = 0;

int terminal_2_flag = 0;
int terminal_3_flag = 0;

char* displayed_video_mem = (char *)VIDEO;

void reset_terminal_pos(){
    terminals[active_terminal].position = -1;
}
/*
 * terminal clear 
 * clears the terminal's screen and sets x and y vectors to point to the top left of the screen
 * uses clear() defined in lib.h
 * INPUTS: NONE
 * OUTPUTS: NONE
 * SIDE EFFECTS: clears 
 */

void clear_terminal(terminal_t term){

    //set the contents of video memory to be blank
    //clear() defined within lib.h
    clear();
    terminals[active_terminal].position = -1;
    // terminals[active_terminal].screen_x = 0;
    // terminals[active_terminal].screen_y = 0;

    //reset the cursor to the top left position. 

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

    enable_cursor();

    terminals[active_terminal].position = -1; // initializing the "actual" buffer size as 0 
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
            terminals[active_terminal].terminal_buffer[i] = '\0';    //setting the static terminal buffer to be null characters
        }
    terminals[active_terminal].terminal_buffer[128] = '\0';
    clear_terminal(terminals[active_terminal]);
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

    if(buf == NULL){
        return -1;
    }
    int bytes_read = 0; //return value, will be updated within the function
    int32_t i;
    int32_t range;
    // int enter_flag = get_enter_flag();
    //check if the last character in the terminal buffer is '\n' (CA suggestion instead of using an enter flag)
    while ( (terminals[active_terminal].terminal_buffer[terminals[active_terminal].position]) != '\n' ){
    /*while (enter_flag == 0) */
    
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

        if( terminals[active_terminal].terminal_buffer[i] != '\n'){

            ((char*) buf)[i] = terminals[active_terminal].terminal_buffer[i];
            // terminal_.terminal_buffer[i] = '\0'; 
            // terminal_.position = terminal_.position - 1;
            bytes_read = bytes_read + 1;
        }

        else{
            terminals[active_terminal].terminal_buffer[i] = '\0';
            break;
        }
            

    }

    //clearing the buffer entirely once read 
    int j;
    for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++){
        terminals[active_terminal].terminal_buffer[j] = '\0';
    }

    terminals[active_terminal].position = -1; 
    // for(j = 0; j < range; j++){

    //     if((j + range) < KEYBOARD_BUFFER_SIZE){
    //         terminal_.terminal_buffer[j] = terminal_.terminal_buffer[j + range];
    //         terminal_.terminal_buffer[j + range] = '\0';
    //     }
        
    // }

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

    //input buffer
    //for loop and put c
    if ( buf == NULL){
        return -1;
    }
    int32_t i;
    char current_character;


    //check the conditions with TAs
    for( i = 0; i < nbytes; i++){
        current_character = ((char *) buf)[i];
        putc(current_character);
       
    }
    // putc('\n');
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

//change to update the buffer of the current index
 void terminal_update_buffer(unsigned char character) {

    if( character == '\n'){
        terminals[active_terminal].position = terminals[active_terminal].position + 1;
        terminals[active_terminal].terminal_buffer[terminals[active_terminal].position] = '\n';
    }
    else if(terminals[active_terminal].position == (KEYBOARD_BUFFER_SIZE -2 )){

        terminals[active_terminal].position = (terminals[active_terminal].position) + 1;
        terminals[active_terminal].terminal_buffer[terminals[active_terminal].position] = character;  
        terminals[active_terminal].terminal_buffer[KEYBOARD_BUFFER_SIZE-1] = '\n';

    }
    else if( (terminals[active_terminal].position < (KEYBOARD_BUFFER_SIZE -1)) ){
        terminals[active_terminal].position = (terminals[active_terminal].position) + 1;         
        terminals[active_terminal].terminal_buffer[terminals[active_terminal].position] = character;
    }
   // what should 
    
 }

 void terminal_remove_from_buffer(){
    if(terminals[active_terminal].position >= 0){
        int delete_pos = terminals[active_terminal].position;
        terminals[active_terminal].terminal_buffer[delete_pos] = '\0';
        terminals[active_terminal].position = terminals[active_terminal].position - 1;
    }
 }

/*
 * terminal close
 * closes the terminal
 * INPUTS: in32_t fd
 * OUTPUTS: returns 0 on success
 */ 
 int terminal_close(int32_t fd){

    int i;

    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
        terminals[active_terminal].terminal_buffer[i] = '\0';
    }

    terminals[active_terminal].position = -1;

    return 0;

 }

/*
 * get_terminal_position
 * returns the terminal position
 * INPUTS: none
 * OUTPUTS: terminal_.position
 * SIDE EFFECTS: returns the terminal buffer position
 */
int get_terminal_position(void){

    return(terminals[active_terminal].position);

}


void switch_terminal(int id){

    int prev_active_terminal = active_terminal;

    // swap_video_memory(id);

    memcpy((char *)terminals[prev_active_terminal].video_memory , (char * ) VIDEO, 4096); // storing the contents of vid memory into the terminal that WAS being displayed

    memcpy((char * ) VIDEO, (char *)terminals[id].video_memory, 4096); //storing the contents of the new terminal to the displayed video memory 

    active_terminal = id; //changing active terminal 

    // clear_terminal_buffer(prev_active_terminal);
    update_cursor( terminals[active_terminal].screen_x, terminals[active_terminal].screen_y); //updating displayed cursor position

    if(active_terminal == 1){
        if (terminal_2_flag == 0){
            execute((const uint8_t *)"shell");
            terminal_2_flag = 1;
        }
    }

    if(active_terminal == 2){
        if (terminal_3_flag == 0){
            execute((const uint8_t *)"shell");
            terminal_3_flag = 1;
        }
    }

    //sav
}

void terminal_init(){
    int i;
    int j;
    int k;
    for(i = 0; i < 3; i++){
        
        terminals[i].screen_x = 0;
        terminals[i].screen_y = 0;
        terminals[i].position = -1;


        // switch (i)
        // {
        // case 0:
        //     terminals[i].video_memory =  TERM1_MEMORY;
            
        //     break;

        // case 1:
        //     terminals[i].video_memory =  TERM2_MEMORY;
        //     Term2PageInit();
        //     break;
        // case 2:
        //     terminals[i].video_memory =  TERM3_MEMORY;
        //     Term3PageInit();
        //     break;

        // default:
        //     break;
        // } 

        if( i == 0){
            terminals[i].video_memory = TERM1_MEMORY;
            Term1PageInit();
        }

        else if( i == 1){
            terminals[i].video_memory = TERM2_MEMORY;
            Term2PageInit();
        }
        
        else if( i==2){
            terminals[i].video_memory = TERM3_MEMORY;
            Term3PageInit();
        }

        for(j = 0; j < KEYBOARD_BUFFER_SIZE; j++){

            terminals[i].terminal_buffer[j] = '\0';
        }

            // for (k = 0; k < NUM_ROWS * NUM_COLS; k++) {
            //     *(uint8_t *)(terminals[i].video_memory + (i << 1)) = ' ';
            //     *(uint8_t *)(terminals[i].video_memory + (i << 1) + 1) = ATTRIB;
            //  }

    }
    
}

void clear_terminal_buffer(int id){

    int j;
    for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++){
        terminals[id].terminal_buffer[j] = '\0';
    }

    terminals[id].position = -1; 
}