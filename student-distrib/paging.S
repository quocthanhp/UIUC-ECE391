#define ASM     1
#include "x86_desc.h"
#include "x86_desc.S"
#include "nPage.h"
#include "nPage.c"

/*taken from osdev enabling paging document*/
.text

ENABLE_PAGING:
    movl %eax, page_directory ;\
    movl cr3, %eax            ;\
    movl %eax, cr0            ;\
    orl %eax, 0x80000001      ;\
    movl cr0, %eax            ;\
    movl %eax, cr4            ;\
    orl %eax, 0x00000010      ;\
    movl cr4, %eax            ;\

.end