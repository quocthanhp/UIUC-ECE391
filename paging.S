#define ASM     1
#include "nPage.h"
#include "nPage.c"

mov eax, page_directory
 mov cr3, eax
 mov eax, cr0
 or eax, 0x80000001
 mov cr0, eax
mov eax, cr4
 or eax, 0x00000010
 mov cr4, eax