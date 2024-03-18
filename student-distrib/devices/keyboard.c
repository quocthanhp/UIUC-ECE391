#include "../lib.h"
#include "keyboard.h"
#include "../i8259.h"
#include "string.h"

/* straight from osDEV */
/* DESC : Init the keyboard
*  INPUT : void
*  OUTPUT : void 
*/
void keyboard_int(void){
    /* google says keybaord is usually irq 1*/
    int nothing = 696969; 
    enable_irq( 1 );
    uint16_t idx;
    uint16_t code = get_key(idx);
    //save data from the respective port
    //call the handler
    // pass the data to the handler 
    uint16_t keycodes[];
        keycodes[0x01] = nothing; //escape pressed
        keycodes[0x02] = '1';
        keycodes[0x03] = '2';
        keycodes[0x04] = '3';
        keycodes[0x05] = '4';
        keycodes[0x06] = '5';
        keycodes[0x07] = '6';
        keycodes[0x08] = '7';
        keycodes[0x09] = '8';
        keycodes[0x0A] = '9';
        keycodes[0x0B] = '0';
        keycodes[0x0C] = '-';
        keycodes[0x0D] = '=';
        keycodes[0x0E] = nothing;//backspace
        keycodes[0x0F] = nothing; //tab
        keycodes[0x10] = 'Q';
        keycodes[0x11] = 'W';
        keycodes[0x12] = 'E';
        keycodes[0x13] = 'R';
        keycodes[0x14] = 'T';
        keycodes[0x15] = 'Y';
        keycodes[0x16] = 'U';
        keycodes[0x17] = 'I';
        keycodes[0x18] = 'O';
        keycodes[0x19] = 'P';
        keycodes[0x1A] = '[';
        keycodes[0x1B] = ']';
        keycodes[0x1C] = nothing;//enter pressed
        keycodes[0x1D] = nothing; //left ctrl
        keycodes[0x1E] = 'A';
        keycodes[0x1F] = 'S';
        keycodes[0x20] = 'D';
        keycodes[0x21] = 'F';
        keycodes[0x22] = 'G';
        keycodes[0x23] = 'H';
        keycodes[0x24] = 'J';
        keycodes[0x25] = 'K';
        keycodes[0x26] = 'L';
        keycodes[0x27] = ';';
        keycodes[0x28] = "'";
        keycodes[0x29] = '`' ;
        keycodes[0x2A] = nothing;//left shift
        keycodes[0x2B] = " '\' ";
        keycodes[0x2C] = "Z";
        keycodes[0x2D] = 'X';
        keycodes[0x2E] = 'C';
        keycodes[0x2F] = 'V';
        keycodes[0x30] = 'B';
        keycodes[0x31] = 'N';
        keycodes[0x32] = 'M';
        keycodes[0x33] = ',';
        keycodes[0x34] = '.';
        keycodes[0x35] = '/';
        keycodes[0x36] = nothing; //right shift
        keycodes[0x37] = '*';
        keycodes[0x38] = nothing; //left alt
        keycodes[0x39] = nothing; //space pressed
        keycodes[0x3A] = nothing; //caps lock
        keycodes[0x3B] = nothing; //F1
        keycodes[0x3C] = nothing; //F2
        keycodes[0x3D] = nothing; //F3
        keycodes[0x3E] = nothing; //F4
        keycodes[0x3F] = nothing; //F5
        keycodes[0x40] = nothing; //F6
        keycodes[0x41] = nothing; //F7
        keycodes[0x42] = nothing; //F8
        keycodes[0x43] = nothing; //F9
        keycodes[0x44] = nothing; //F10
        keycodes[0x45] = nothing; //num lock
        keycodes[0x46] = nothing; //scroll lock
        keycodes[0x47] = '7';
        keycodes[0x48] = '8';
        keycodes[0x49] = '9';
        keycodes[0x4A] = '-';
        keycodes[0x4B] = '4';
        keycodes[0x4C] = '5';
        keycodes[0x4D] = '6';
        keycodes[0x4E] = '+';
        keycodes[0x4F] = '1';
        keycodes[0x50] = '2';
        keycodes[0x51] = '3';
        keycodes[0x52] = '0';
        keycodes[0x53] = '.';
        keycodes[0x54] = NULL;
        keycodes[0x55] = NULL;
        keycodes[0x56] = NULL;
        keycodes[0x57] = nothing; //F11
        keycodes[0x58] = nothing; //F12
        keycodes[0x59] =NULL;
        keycodes[0x5A] =NULL;
        keycodes[0x5B] =NULL;
        keycodes[0x5C] =NULL;
        keycodes[0x5D] =NULL;
        keycodes[0x5E] =NULL;
        keycodes[0x5F] =NULL;
        keycodes[0x60] =NULL;
        keycodes[0x61] =NULL;
        keycodes[0x62] =NULL;
        keycodes[0x63] =NULL;
        keycodes[0x64] =NULL;
        keycodes[0x65] =NULL;
        keycodes[0x66] =NULL;
        keycodes[0x67] =NULL;
        keycodes[0x68] =NULL;
        keycodes[0x69] =NULL;
        keycodes[0x6A] =NULL;
        keycodes[0x6B] =NULL;
        keycodes[0x6C] =NULL;
        keycodes[0x6D] =NULL;
        keycodes[0x6E] =NULL;
        keycodes[0x6F] =NULL;
        keycodes[0x70] =NULL;
        keycodes[0x71] =NULL;
        keycodes[0x72] =NULL;
        keycodes[0x73] =NULL;
        keycodes[0x74] =NULL;
        keycodes[0x75] =NULL;
        keycodes[0x76] =NULL;
        keycodes[0x77] =NULL;
        keycodes[0x78] =NULL;
        keycodes[0x79] =NULL;
        keycodes[0x7A] =NULL;
        keycodes[0x7B] =NULL;
        keycodes[0x7C] =NULL;
        keycodes[0x7D] =NULL;
        keycodes[0x7E] =NULL;
        keycodes[0x7F] =NULL;
        keycodes[0x80] =NULL;
        keycodes[0x81] =NULL;
        keycodes[0x82] =NULL;
        keycodes[0x83] =NULL;
        keycodes[0x84] =NULL;
        keycodes[0x85] =NULL;
        keycodes[0x86] =NULL;
        keycodes[0x87] =NULL;
        keycodes[0x88] =NULL;
        keycodes[0x89] =NULL;
        keycodes[0x8A] =NULL;
        keycodes[0x8B] =NULL;
        keycodes[0x8C] =NULL;
        keycodes[0x8D] =NULL;
        keycodes[0x8E] =NULL;
        keycodes[0x8F] =NULL;
        keycodes[0x90] =NULL;
        keycodes[0x91] =NULL;
        keycodes[0x92] =NULL;
        keycodes[0x93] =NULL;
        keycodes[0x94] =NULL;
        keycodes[0x95] =NULL;
        keycodes[0x96] =NULL;
        keycodes[0x97] =NULL;
        keycodes[0x98] =NULL;
        keycodes[0x99] =NULL;
        keycodes[0x9A] =NULL;
        keycodes[0x9B] =NULL;
        keycodes[0x9C] =NULL;
        keycodes[0x9D] =NULL;
        keycodes[0x9E] =NULL;
        keycodes[0x9F] =NULL;
        keycodes[0xA0] =NULL;
        keycodes[0xA1] =NULL;
        keycodes[0xA2] =NULL;
        keycodes[0xA3] =NULL;
        keycodes[0xA4] =NULL;
        keycodes[0xA5] =NULL;
        keycodes[0xA6] =NULL;
        keycodes[0xA7] =NULL;
        keycodes[0xA8] =NULL;
        keycodes[0xA9] =NULL;
        keycodes[0xAA] =NULL;
        keycodes[0xAB] =NULL;
        keycodes[0xAC] =NULL;
        keycodes[0xAD] =NULL;
        keycodes[0xAE] =NULL;
        keycodes[0xAF] =NULL;
        keycodes[0xB0] = NULL;
        keycodes[0xB1] =NULL;
        keycodes[0xB2] =NULL;
        keycodes[0xB3] =NULL;
        keycodes[0xB4] =NULL;
        keycodes[0xB5] =NULL;
        keycodes[0xB6] =NULL;
        keycodes[0xB7] =NULL;
        keycodes[0xB8] =NULL;
        keycodes[0xB9] =NULL;
        keycodes[0xBA] =NULL;
        keycodes[0xBB] =NULL;
        keycodes[0xBC] =NULL;
        keycodes[0xBD] =NULL;
        keycodes[0xBE] =NULL;
        keycodes[0xBF] =NULL;
        keycodes[0xC0] = NULL;
        keycodes[0xC1] =NULL;
        keycodes[0xC2] =NULL;
        keycodes[0xC3] =NULL;
        keycodes[0xC4] =NULL;
        keycodes[0xC5] =NULL;
        keycodes[0xC6] =NULL;
        keycodes[0xC7] =NULL;
        keycodes[0xC8] =NULL;
        keycodes[0xC9] =NULL;
        keycodes[0xCA] =NULL;
        keycodes[0xCB] =NULL;
        keycodes[0xCC] =NULL;
        keycodes[0xCD] =NULL;
        keycodes[0xCE] =NULL;
        keycodes[0xCF] =NULL;
        keycodes[0xD0] = NULL;
        keycodes[0xD1] =NULL;
        keycodes[0xD2] =NULL;
        keycodes[0xD3] =NULL;
        keycodes[0xD4] =NULL;
        keycodes[0xD5] =NULL;
        keycodes[0xD6] =NULL;
        keycodes[0xD7] =NULL;
}

uint16_t get_key(void){
    uint16_t code;
    inb(Keyboard_port, code);

    return code;

}