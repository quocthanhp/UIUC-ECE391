#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "syscall.h"
/* constants to be used within terminal */
//values taken from discussion slides
#define TERMINAL_COLUMNS 80
#define TERMINAL_ROWS 25
#define KEYBOARD_BUFFER_SIZE 128
#define MAX_TERMINALS   3

#define TERMINAL_ONE  0
#define TERMINAL_TWO  1
#define TERMINAL_THREE 2


typedef struct terminal{
    char terminal_buffer[KEYBOARD_BUFFER_SIZE];
    int screen_x;
    int screen_y;
    int position;
    //number of processes runnning
    uint32_t video_memory;
    int processes[MAX_PROCESSES];
    int active_process; 
    uint32_t ebp;
    uint32_t esp;

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
