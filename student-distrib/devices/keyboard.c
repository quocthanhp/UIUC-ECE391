#include "../lib.h"
#include "keyboard.h"
#include "../i8259.h"
#include "../terminal.h"
// #include "string.h"
// #include "../lib.c" 

extern terminal_t terminals[3]; // make array of 3
extern int active_terminal;

int caps_lock_flag; //turned off (0) by default 
int shift_flag;
int alt_flag;
int backspace_flag;
int control_flag;
int l_flag;
int enter_flag;
int f1_flag;
int f2_flag;
int f3_flag;

unsigned char lowercase_characters[MAX_SCAN_CODES] = {'\0' , '\0' /*escape*/, '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , BACKSPACE_PRESSED , TAB_PRESSED  , 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '\n' , '\0' , 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , ';' , '\'', '`' , '\0' /*left shift*/, '\\' , 'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , '.' , '/' , '\0' , '\0' , '\0' , ' ' , '\0' , F1_PRESSED, F2_PRESSED, F3_PRESSED};

unsigned char upppercase_characters[MAX_SCAN_CODES]= {'\0' , '\0' /*escape*/, '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , BACKSPACE_PRESSED , TAB_PRESSED  , 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , '\n' , '\0' , 'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J'  , 'K' , 'L' , ':' , '"' , '~' , '\0' /*left shift*/, '|'  , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '<' , '>' , '?' , '\0' , '\0' , '\0' , ' ' , '\0' , F1_PRESSED, F2_PRESSED, F3_PRESSED};

/*flags to be used for special cases*/
// int caps_lock_flag = 0; //turned off (0) by default 
// int shift_flag;
// int alt_flag;
// int backspace_flag;
// int control_flag;
// int backspace_flag;
// int l_flag;
// int enter_flag =0;
/* straight from osDEV */
/* DESC : Init the keyboard
*  INPUT : void
*  OUTPUT : void 
*/
void keyboard_init(void){
    caps_lock_flag = 0; //turned off (0) by default 
    shift_flag = 0;
    alt_flag = 0;
    backspace_flag = 0;
    control_flag = 0;
    backspace_flag = 0;
    l_flag = 0;
    enter_flag = 0;
    f1_flag = 0;
    f2_flag = 0;
    f3_flag = 0;
    reset_terminal_pos();
    enable_irq(1);
}

/* keybaord interrupt function
*  print scancode to screen each time a key is pressed
*
*/
int nothing = NULL; 
void keyboard_interrupt(void){

/* google says keybaord is usually irq 1*/ 
    // cli();
    send_eoi(1);
    uint8_t scan_code = get_key();

    //case statement 
    //special key case   
    //default printing letters and numbers
     
    switch (scan_code){
        case CAPS_LOCK_PRESSED:
            //switches the caps loack flag to 1 or 0
            caps_lock_flag = caps_lock_flag == 0 ? 1 : 0;
            break;
        case LEFT_SHIFT_PRESSED:

            shift_flag = 1;
            //set shift flag to 1
            send_eoi(1);
            return;
        case RIGHT_SHIFT_PRESSED:
            //set shift flag to 1
            shift_flag = 1;
            send_eoi(1);
            return;
        case LEFT_SHIFT_RELEASED:
            //set shift flag to 0
            shift_flag = 0;
            break;
        case RIGHT_SHIFT_RELEASED:
            //set shift flag to 0
            shift_flag = 0;
            break;
        case LEFT_ALT_PRESSED:
            //set alt flag to 1
            alt_flag = 1;
            break;
        case LEFT_ALT_RELEASED:
            //set alt flag to 0
            alt_flag = 0; 
            break;
        case BACKSPACE_PRESSED:
            backspace_flag = 1;
            break;
        case BACKSPACE_RELEASED:
            backspace_flag = 0;
            break;
        case LEFT_CONTROL_PRESSED:
            control_flag = 1;
            break;

        case LEFT_CONTROL_RELEASED:
            control_flag = 0;
            break;

        case L_PRESSED:
            l_flag = 1;
            break;
        case L_RELEASED:
            l_flag = 0;
            break;
        case ENTER_PRESSED:
            enter_flag =1; 
            break;
        case ENTER_RELEASED:
            enter_flag = 0;
            break;
        case F1_PRESSED:
            f1_flag = 1;
            break;
        case F2_PRESSED:
            f2_flag = 1;
            break;
        case F3_PRESSED:
            f3_flag = 1;
            break;
        case F1_RELEASED:
            f1_flag = 0;
            break;
        case F2_RELEASED:
            f2_flag = 0;
            break;
        case F3_RELEASED:
            f3_flag = 0;
            break;
        default:
            break;
    }

    
    if (scan_code < MAX_SCAN_CODES && scan_code > 0) {

        //clear screen
        if(control_flag == 1){
            if(scan_code == L_PRESSED){
                clear_terminal();
            }
            return;
        }

        /*switch to respective terminal*/
        if(alt_flag == 1){
            if(scan_code == F1_PRESSED){

                //perform context switch 
                register uint32_t prev_terminal_saved_ebp asm("ebp");
                pcb_t * current_pcb = get_current_pcb();
                current_pcb->ebp = prev_terminal_saved_ebp;

                register uint32_t prev_terminal_saved_esp asm("esp"); 
                current_pcb->esp = saved_esp;

                int pid = current_pcb->pid;
                tss.ss0 = KERNEL_DS;
                tss.esp0 = (KERNAL_STACK - pid * KERNEL_STACK_SIZE) - 4;
                
                asm volatile ("                 \n\
                    pushl    %%eax          \n\
                    pushl    %%ebx          \n\
                    pushfl                  \n\
                    popl     %%eax          \n\
                    orl      $0x200,%%eax   \n\
                    pushl    %%eax          \n\
                    pushl    %%ecx          \n\
                    pushl    %%edx          \n\
                    "
                    :
                    : "a" (USER_DS), "b" (PROGRAM_STACK_VIRTUAL - 4), "c" (USER_CS), "d" (prog_entry) 
                    : "memory"
                );

                asm volatile ("iret");
                switch_terminal(0);
                return;
            }
            if(scan_code == F2_PRESSED){
                switch_terminal(1);
                return;
            }
            if(scan_code == F3_PRESSED){
                switch_terminal(2);
                return;
            }
            return;
        }

        if(scan_code == BACKSPACE_PRESSED){
            int pos;
            pos = get_terminal_position();
            if( pos >= 0){
                terminal_remove_from_buffer();
                putc(BACKSPACE_PRESSED);
            }
            
        }

        else if(scan_code == ENTER_PRESSED){
            putc(upppercase_characters[scan_code]);
            terminal_update_buffer(upppercase_characters[scan_code]);
        }

        /*default case for printing any character*/
        else if(caps_lock_flag == 0) {
            //shift pressed while caps lock is off
            if(shift_flag == 1){
                if (terminals[active_terminal].position != (KEYBOARD_BUFFER_SIZE -2)){
                    putc(upppercase_characters[scan_code]);
                    terminal_update_buffer(upppercase_characters[scan_code]);
                }
                
            }

            //shift isn't pressed while caps lock is off
            else if (shift_flag == 0){
                if ( terminals[active_terminal].position != (KEYBOARD_BUFFER_SIZE -2) ){
                    putc(lowercase_characters[scan_code]);
                    terminal_update_buffer(lowercase_characters[scan_code]);
                }
            }
        }
        else if(caps_lock_flag == 1){

            //shift pressed while caps lock is on
            if(shift_flag == 1){
                if ( terminals[active_terminal].position != (KEYBOARD_BUFFER_SIZE -2) ){
                    putc(lowercase_characters[scan_code]);
                    terminal_update_buffer(lowercase_characters[scan_code]);
                }
            }
            else{
                if ( terminals[active_terminal].position != (KEYBOARD_BUFFER_SIZE -2) ){
                    putc(upppercase_characters[scan_code]);
                    terminal_update_buffer(upppercase_characters[scan_code]);
                }
            }
        }


    }
     
}



/* helper */
uint8_t get_key(void){
    uint8_t code;
    code = inb(Keyboard_port);

    return code;

}
