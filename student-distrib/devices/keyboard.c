#include "../lib.h"
#include "keyboard.h"
#include "../i8259.h"
#include "../terminal.h"
// #include "string.h"
// #include "../lib.c" 


int caps_lock_flag; //turned off (0) by default 
int shift_flag;
int alt_flag;
int backspace_flag;
int control_flag;
int l_flag;
int enter_flag;

unsigned char lowercase_characters[MAX_SCAN_CODES] = {'\0' , '\0' /*escape*/, '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , BACKSPACE_PRESSED , TAB_PRESSED  , 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '\n' , '\0' , 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j' , 'k' , 'l' , ';' , '\'', '`' , '\0' /*left shift*/, '\\' , 'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , '.' , '\0' , '\0' , '\0' , '\0' , ' '};

unsigned char upppercase_characters[MAX_SCAN_CODES]= {'\0' , '\0' /*escape*/, '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , BACKSPACE_PRESSED , TAB_PRESSED  , 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , '\n' , '\0' , 'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J'  , 'K' , 'L' , ':' , '"' , '~' , '\0' /*left shift*/, '|'  , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '<' , '>' , '\0' , '\0' , '\0' , '\0' , ' '};

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
    enable_irq(1);
}

/* keybaord interrupt function
*  print scancode to screen each time a key is pressed
*
*/
int nothing = NULL; 
void keyboard_interrupt(void){

/* google says keybaord is usually irq 1*/ 
    cli();

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
            // shift_flag = shift_flag == 0 ? 1 : 0;
            shift_flag = 1;
            //set shift flag to 1
            send_eoi(1);
            sti();
            return;
        case RIGHT_SHIFT_PRESSED:
            //set shift flag to 1
            shift_flag = 1;
            send_eoi(1);
            sti();
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
            alt_flag = 0;
        /*case LEFT_ALT_RELEASED:
            //set alt flag to 0
            alt_flag = 0; */
            break;
        case BACKSPACE_PRESSED:
            backspace_flag = 1;
            terminal_remove_from_buffer();
            break;
        case BACKSPACE_RELEASED:
            backspace_flag = 0;
            break;
        case LEFT_CONTROL_PRESSED:
            control_flag = 1;
            break;
        // case RIGHT_CONTROL_PRESSED:
        //     control_flag = 1;
        
        case LEFT_CONTROL_RELEASED:
            control_flag = 0;
            break;
        // case RIGHT_CONTROL_RELEASED:
        //     control_flag = 0;

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
        default:
            break;
    }

    if (scan_code < MAX_SCAN_CODES && scan_code > 0) {

        if(scan_code == BACKSPACE_PRESSED){
            putc(lowercase_characters[scan_code]);
        }
        else if(caps_lock_flag == 0) {
            //shift pressed while caps lock is off
            if(shift_flag == 1){
                //
                putc(upppercase_characters[scan_code]);
                terminal_update_buffer(upppercase_characters[scan_code]);
            }

            //shift isn't pressed while caps lock is off
            else if (shift_flag == 0){
                putc(lowercase_characters[scan_code]);
                //function to add to buffer
                terminal_update_buffer(lowercase_characters[scan_code]);
            }
        }
        else if(caps_lock_flag == 1){

            //shift pressed while caps lock is on
            if(shift_flag == 1){
                putc(lowercase_characters[scan_code]);
                terminal_update_buffer(lowercase_characters[scan_code]);
            }
            else{
            //else if(shift_flag == 0)
                // putc('!');
                putc(upppercase_characters[scan_code]);
                terminal_update_buffer(upppercase_characters[scan_code]);
            }
        }

        if(control_flag == 1){
            if(scan_code == L_PRESSED){
                clear_terminal();
            }
        }
    }
     
    /* after each key send eoi */
    send_eoi(1);
    sti();
    //update terminal buffer 
    //print the actual character as well using putc 
}


int get_enter_flag(){
    return(enter_flag);

}


/* helper */
uint8_t get_key(void){
    uint8_t code;
    code = inb(Keyboard_port);

    return code;

}
