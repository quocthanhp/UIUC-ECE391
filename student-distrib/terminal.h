#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
/* constants to be used within terminal */
//values taken from discussion slides
#define TERMINAL_COLUMNS 80
#define TERMINAL_ROWS 25
#define KEYBOARD_BUFFER_SIZE 128
#define MAX_TERMINALS   3

typedef struct terminal{
    char terminal_buffer[KEYBOARD_BUFFER_SIZE];
    int screen_x;
    int screen_y;
    int position;
    //number of processes runnning
    uint32_t video_memory;
    int processes[MAX_PROCESSES -1];
    int active_process; 

} terminal_t;


void clear_terminal();
int terminal_open(const uint8_t * filename);
int terminal_close(int32_t fd);
int terminal_read(int32_t fd, void * buf, int32_t nbytes);
int terminal_write(int32_t fd, const void * buf, int32_t nbytes);
void terminal_update_buffer(unsigned char character);
void terminal_remove_from_buffer();
int get_terminal_position();
void reset_terminal_pos();
void switch_terminal(int id);
void terminal_init();

#endif /* _TERMINAL_H */
