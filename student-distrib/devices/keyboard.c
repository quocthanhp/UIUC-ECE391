#include "../lib.h"
#include "keyboard.h"
#include "../i8259.h"
#include "../terminal.h"
// #include "string.h"
// #include "../lib.c" 



unsigned char lowercase_characters[53] = {'\0' , '\0' /*escape*/, '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' , '-' , '=' , BACKSPACE_PRESSED , TAB_PRESSED  , 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' , 'o' , 'p' , '[' , ']' , '\n' , '\0' , 'a' , 's' , 'd' , 'f' , 'g' , 'h' , 'j ' , 'k' , 'l' , ';' , '\'', '`' , '\0' /*left shift*/, '\\' , 'z' , 'x' , 'c' , 'v' , 'b' , 'n' , 'm' , ',' , '.' };

unsigned char upppercase_characters[53]= {'\0' , '\0' /*escape*/, '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' , '_' , '+' , BACKSPACE_PRESSED , TAB_PRESSED  , 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' , 'O' , 'P' , '{' , '}' , '\n' , '\0' , 'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J'  , 'K' , 'L' , ':' , '"' , '~' , '\0' /*left shift*/, '|'  , 'Z' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , '<' , '>' };

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
    // cli();

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
            break;
        case RIGHT_SHIFT_PRESSED:
            //set shift flag to 1
            shift_flag = 1;
            break;
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

    if(caps_lock_flag == 0) {

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
                //else if(shift_flag == 0){
                    // putc('!');
                    putc(upppercase_characters[scan_code]);
                    terminal_update_buffer(upppercase_characters[scan_code]);
                }
            }


            // else if(shift_flag == 0) {
            //     putc(lowercase_characters[scan_code]);
            //     //function to add to buffer
            //     terminal_update_buffer(lowercase_characters[scan_code]);
            // }

            // else if(caps_lock_flag == 1){
            //     putc(uppercase_characters[scan_code]);
            //     //function to add to buffer
            //     terminal_update_buffer(lowercase_characters[scan_code]);
            // }

            // else if(shift_flag == 1){
            //     putc(uppercase_characters[scan_code]);
            //     //function to add to buffer
            //     terminal_update_buffer(lowercase_characters[scan_code]);
            // }

            else if(control_flag == 1){
                if(scan_code == L_PRESSED){
                    clear_terminal();
                }
            }
            // else if(backspace_flag ==1){
            //     putc(BACKSPACE_PRESSED);
            // }
   // print_code(scan_code);
    
    /* after each key send eoi */
    send_eoi(1);
    // sti();
    //update terminal buffer 
    //print the actual character as well using putc 
}

int get_enter_flag(){
    return(enter_flag);

}
// void print_code(uint8_t scan_code){
//     switch (scan_code)
//     {
//     case 0x01:
//         printf("%d", 1);
//         break;
//     case 0x02:
//         printf("%d", 2);
//         break;
//     case 0x03:
//         printf("%d", 3);
//         break;
//      case 0x04:
//         printf("%d", 4);
//         break;
//     case 0x05:
//         printf("%d", 5);
//         break;   
//     case 0x06:
//         printf("%d", 6);
//         break;
//     case 0x07:
//         printf("%d", 7);
//         break;
//     case 0x08:
//         printf("%d", 8);
//         break;
//     case 0x09:
//         printf("%d", 9);
//         break;
//     case 0x0A:
//         printf("%d", 0);
//         break;
//     case 0x0B:
//         printf("-");
//         break;
//     case 0x0C:
//         printf("=");
//         break;
//      case 0x0D:
//         printf("%d", nothing);
//         break;
//     case 0x0E:
//         printf("%d", nothing);
//         break;   
//     case 0x0F:
//         printf("%d", nothing);
//         break;
//     case 0x10:
//         printf("q");
//         break;
//     case 0x11:
//         printf("w");
//         break;
//     case 0x12:
//         printf("e");
//         break;   
//     case 0x13:
//         printf("r");
//         break;
//     case 0x14:
//         printf("t");
//         break;
//     case 0x15:
//         printf("y");
//         break;
//      case 0x16:
//         printf("u");
//         break;
//     case 0x17:
//         printf("i");
//         break;   
//     case 0x18:
//         printf("o");
//         break;
//     case 0x19:
//         printf("p");
//         break;
//     case 0x1A:
//         printf("[");
//         break;
//     case 0x1B:
//         printf("]");
//         break;
//     case 0x1D:
//         printf("%d", nothing);
//         break;
//     case 0x1E:
//         printf("a");
//         break;
//     case 0x1F:
//         printf("s");
//         break;
//     case 0x20:
//         printf("d");
//         break;   
//     case 0x21:
//         printf("f");
//         break;
//     case 0x22:
//         printf("g");
//         break;
//     case 0x23:
//         printf("h");
//         break;
//     case 0x24:
//         printf("j");
//         break; 
//     case 0x25:
//         printf("k");
//         break;
//     case 0x26:
//         printf("l");
//         break;
//     case 0x27:
//         printf(";");
//         break;
//      case 0x28:
//         printf("'");
//         break;
//     case 0x29:
//         printf("`");
//         break;   
//     case 0x2A:
//         printf("%d", nothing);
//         break;
//     case 0x2B:
//         printf("'\'");
//         break;
//     case 0x2C:
//         printf("z");
//         break;
//     case 0x2D:
//         printf("x");
//         break;
//     case 0x2E:
//         printf("c");
//         break;
//     case 0x2F:
//         printf("v");
//         break;
//     case 0x30:
//         printf("b");
//         break;
//     case 0x31:
//         printf("n");
//         break;
//     case 0x32:
//         printf("m");
//         break;   
//     case 0x33:
//         printf(",");
//         break;
//     case 0x34:
//         printf(".");
//         break;
//     case 0x35:
//         printf("/");
//         break;
//     case 0x36:
//         printf("%d", nothing);
//         break; 
//     case 0x37:
//         printf("*");
//         break;
//     case 0x38:
//         printf("%d", nothing);
//         break;
//     case 0x39:
//         printf("%d", nothing);
//         break;
//     case 0x3A:
//         printf("%d", nothing);
//         break;
//     case 0x3B:
//         printf("%d", nothing);
//         break;   
//     case 0x3C:
//         printf("%d", nothing);
//         break;
//     case 0x3D:
//         printf("%d", nothing);
//         break;
//     case 0x3E:
//         printf("%d", nothing);
//         break;
//     case 0x3F:
//         printf("%d", nothing);
//         break;
//     case 0x40:
//         printf("%d", nothing);
//         break;
//     case 0x41:
//         printf("%d", nothing);
//         break;
//     case 0x42:
//         printf("%d", nothing);
//         break;
//     case 0x43:
//         printf("%d", nothing);
//         break;
//     case 0x44:
//        printf("%d", nothing);
//         break;   
//     case 0x45:
//         printf("%d", nothing);
//         break;
//     case 0x46:
//         printf("%d", nothing);
//         break;
//     case 0x47:
//         printf("%d", 7);
//         break;
//     case 0x48:
//         printf("%d", 8);
//         break; 
//     case 0x49:
//         printf("%d", 9);
//         break;
//     case 0x4A:
//         printf("-");
//         break;
//     case 0x4B:
//         printf("%d", 4);
//         break;
//      case 0x4C:
//         printf("%d", 5);
//         break;
//     case 0x4D:
//         printf("%d", 6);
//         break;   
//     case 0x4E:
//         printf("+");
//         break;
//     case 0x4F:
//         printf("%d", 1);
//         break;
//     case 0x50:
//         printf("%d", 2);
//         break;
//     case 0x51:
//         printf("%d", 3);
//         break;
//     case 0x52:
//         printf("%d", 0);
//         break;
//     // case 0x53:
//     //     printf(".");
//     //     break;
//     // case 0x54:
//     //     printf("%d", 1);
//     //     break;
//     //  case 0x55:
//     //     printf("%d", 1);
//     //     break;
//     case 0x56:
//         printf("%d", nothing);
//         break;   
//     case 0x57:
//         printf("%d", nothing);
//         break;
//     default:
        
//         break;
//     }

// }
/*2 buffers for lowercase and uppercase set up flags */

//char keycodes[0xD7][1];
    //     keycodes[0x01] = nothing; //escape pressed
    //     keycodes[0x02] = '1';
    //     keycodes[0x03] = '2';
    //     keycodes[0x04] = '3';
    //     keycodes[0x05] = '4';
    //     keycodes[0x06] = '5';
    //     keycodes[0x07] = '6';
    //     keycodes[0x08] = '7';
    //     keycodes[0x09] = '8';
    //     keycodes[0x0A] = '9';
    //     keycodes[0x0B] = '0';
    //     keycodes[0x0C] = '-';
    //     keycodes[0x0D] = '=';
    //     keycodes[0x0E] = nothing; //backspace
    //     keycodes[0x0F] = nothing; //tab
    //     keycodes[0x10] = 'Q';
    //     keycodes[0x11] = 'W';
    //     keycodes[0x12] = 'E';
    //     keycodes[0x13] = 'R';
    //     keycodes[0x14] = 'T';
    //     keycodes[0x15] = 'Y';
    //     keycodes[0x16] = 'U';
    //     keycodes[0x17] = 'I';
    //     keycodes[0x18] = 'O';
    //     keycodes[0x19] = 'P';
    //     keycodes[0x1A] = '[';
    //     keycodes[0x1B] = ']';
    //     keycodes[0x1C] = nothing; //enter pressed
    //     keycodes[0x1D] = nothing; //left ctrl
    //     keycodes[0x1E] = 'A';
    //     keycodes[0x1F] = 'S';
    //     keycodes[0x20] = 'D';
    //     keycodes[0x21] = 'F';
    //     keycodes[0x22] = 'G';
    //     keycodes[0x23] = 'H';
    //     keycodes[0x24] = 'J';
    //     keycodes[0x25] = 'K';
    //     keycodes[0x26] = 'L';
    //     keycodes[0x27] = ';';
    //     keycodes[0x28] = "'";
    //     keycodes[0x29] = '`' ;
    //     keycodes[0x2A] = nothing; //left shift
    //     keycodes[0x2B] = " '\' ";
    //     keycodes[0x2C] = "Z";
    //     keycodes[0x2D] = 'X';
    //     keycodes[0x2E] = 'C';
    //     keycodes[0x2F] = 'V';
    //     keycodes[0x30] = 'B';
    //     keycodes[0x31] = 'N';
    //     keycodes[0x32] = 'M';
    //     keycodes[0x33] = ',';
    //     keycodes[0x34] = '.';
    //     keycodes[0x35] = '/';
    //     keycodes[0x36] = nothing; //right shift
    //     keycodes[0x37] = '*';
    //     keycodes[0x38] = nothing; //left alt
    //     keycodes[0x39] = nothing; //space pressed
    //     keycodes[0x3A] = nothing; //caps lock
    //     keycodes[0x3B] = nothing; //F1
    //     keycodes[0x3C] = nothing; //F2
    //     keycodes[0x3D] = nothing; //F3
    //     keycodes[0x3E] = nothing; //F4
    //     keycodes[0x3F] = nothing; //F5
    //     keycodes[0x40] = nothing; //F6
    //     keycodes[0x41] = nothing; //F7
    //     keycodes[0x42] = nothing; //F8
    //     keycodes[0x43] = nothing; //F9
    //     keycodes[0x44] = nothing; //F10
    //     keycodes[0x45] = nothing; //num lock
    //     keycodes[0x46] = nothing; //scroll lock
    //     keycodes[0x47] = '7';
    //     keycodes[0x48] = '8';
    //     keycodes[0x49] = '9';
    //     keycodes[0x4A] = '-';
    //     keycodes[0x4B] = '4';
    //     keycodes[0x4C] = '5';
    //     keycodes[0x4D] = '6';
    //     keycodes[0x4E] = '+';
    //     keycodes[0x4F] = '1';
    //     keycodes[0x50] = '2';
    //     keycodes[0x51] = '3';
    //     keycodes[0x52] = '0';
    //     keycodes[0x53] = '.';
    //     keycodes[0x54] = NULL;
    //     keycodes[0x55] = NULL;
    //     keycodes[0x56] = NULL;
    //     keycodes[0x57] = nothing; //F11
    //     keycodes[0x58] = nothing; //F12
    //     keycodes[0x59] =NULL;
    //     keycodes[0x5A] =NULL;
    //     keycodes[0x5B] =NULL;
    //     keycodes[0x5C] =NULL;
    //     keycodes[0x5D] =NULL;
    //     keycodes[0x5E] =NULL;
    //     keycodes[0x5F] =NULL;
    //     keycodes[0x60] =NULL;
    //     keycodes[0x61] =NULL;
    //     keycodes[0x62] =NULL;
    //     keycodes[0x63] =NULL;
    //     keycodes[0x64] =NULL;
    //     keycodes[0x65] =NULL;
    //     keycodes[0x66] =NULL;
    //     keycodes[0x67] =NULL;
    //     keycodes[0x68] =NULL;
    //     keycodes[0x69] =NULL;
    //     keycodes[0x6A] =NULL;
    //     keycodes[0x6B] =NULL;
    //     keycodes[0x6C] =NULL;
    //     keycodes[0x6D] =NULL;
    //     keycodes[0x6E] =NULL;
    //     keycodes[0x6F] =NULL;
    //     keycodes[0x70] =NULL;
    //     keycodes[0x71] =NULL;
    //     keycodes[0x72] =NULL;
    //     keycodes[0x73] =NULL;
    //     keycodes[0x74] =NULL;
    //     keycodes[0x75] =NULL;
    //     keycodes[0x76] =NULL;
    //     keycodes[0x77] =NULL;
    //     keycodes[0x78] =NULL;
    //     keycodes[0x79] =NULL;
    //     keycodes[0x7A] =NULL;
    //     keycodes[0x7B] =NULL;
    //     keycodes[0x7C] =NULL;
    //     keycodes[0x7D] =NULL;
    //     keycodes[0x7E] =NULL;
    //     keycodes[0x7F] =NULL;
    //     keycodes[0x80] =NULL;
    //     keycodes[0x81] =NULL;
    //     keycodes[0x82] =NULL;
    //     keycodes[0x83] =NULL;
    //     keycodes[0x84] =NULL;
    //     keycodes[0x85] =NULL;
    //     keycodes[0x86] =NULL;
    //     keycodes[0x87] =NULL;
    //     keycodes[0x88] =NULL;
    //     keycodes[0x89] =NULL;
    //     keycodes[0x8A] =NULL;
    //     keycodes[0x8B] =NULL;
    //     keycodes[0x8C] =NULL;
    //     keycodes[0x8D] =NULL;
    //     keycodes[0x8E] =NULL;
    //     keycodes[0x8F] =NULL;
    //     keycodes[0x90] =NULL;
    //     keycodes[0x91] =NULL;
    //     keycodes[0x92] =NULL;
    //     keycodes[0x93] =NULL;
    //     keycodes[0x94] =NULL;
    //     keycodes[0x95] =NULL;
    //     keycodes[0x96] =NULL;
    //     keycodes[0x97] =NULL;
    //     keycodes[0x98] =NULL;
    //     keycodes[0x99] =NULL;
    //     keycodes[0x9A] =NULL;
    //     keycodes[0x9B] =NULL;
    //     keycodes[0x9C] =NULL;
    //     keycodes[0x9D] =NULL;
    //     keycodes[0x9E] =NULL;
    //     keycodes[0x9F] =NULL;
    //     keycodes[0xA0] =NULL;
    //     keycodes[0xA1] =NULL;
    //     keycodes[0xA2] =NULL;
    //     keycodes[0xA3] =NULL;
    //     keycodes[0xA4] =NULL;
    //     keycodes[0xA5] =NULL;
    //     keycodes[0xA6] =NULL;
    //     keycodes[0xA7] =NULL;
    //     keycodes[0xA8] =NULL;
    //     keycodes[0xA9] =NULL;
    //     keycodes[0xAA] =NULL;
    //     keycodes[0xAB] =NULL;
    //     keycodes[0xAC] =NULL;
    //     keycodes[0xAD] =NULL;
    //     keycodes[0xAE] =NULL;
    //     keycodes[0xAF] =NULL;
    //     keycodes[0xB0] = NULL;
    //     keycodes[0xB1] =NULL;
    //     keycodes[0xB2] =NULL;
    //     keycodes[0xB3] =NULL;
    //     keycodes[0xB4] =NULL;
    //     keycodes[0xB5] =NULL;
    //     keycodes[0xB6] =NULL;
    //     keycodes[0xB7] =NULL;
    //     keycodes[0xB8] =NULL;
    //     keycodes[0xB9] =NULL;
    //     keycodes[0xBA] =NULL;
    //     keycodes[0xBB] =NULL;
    //     keycodes[0xBC] =NULL;
    //     keycodes[0xBD] =NULL;
    //     keycodes[0xBE] =NULL;
    //     keycodes[0xBF] =NULL;
    //     keycodes[0xC0] = NULL;
    //     keycodes[0xC1] =NULL;
    //     keycodes[0xC2] =NULL;
    //     keycodes[0xC3] =NULL;
    //     keycodes[0xC4] =NULL;
    //     keycodes[0xC5] =NULL;
    //     keycodes[0xC6] =NULL;
    //     keycodes[0xC7] =NULL;
    //     keycodes[0xC8] =NULL;
    //     keycodes[0xC9] =NULL;
    //     keycodes[0xCA] =NULL;
    //     keycodes[0xCB] =NULL;
    //     keycodes[0xCC] =NULL;
    //     keycodes[0xCD] =NULL;
    //     keycodes[0xCE] =NULL;
    //     keycodes[0xCF] =NULL;
    //     keycodes[0xD0] = NULL;
    //     keycodes[0xD1] =NULL;
    //     keycodes[0xD2] =NULL;
    //     keycodes[0xD3] =NULL;
    //     keycodes[0xD4] =NULL;
    //     keycodes[0xD5] =NULL;
    //     keycodes[0xD6] =NULL;
    //     keycodes[0xD7] =NULL;


/* helper */
uint8_t get_key(void){
    uint8_t code;
    code = inb(Keyboard_port);

    return code;

}
