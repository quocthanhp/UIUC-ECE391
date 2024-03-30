#define ASM 1
#include "x86_desc.h"
.GLOBAL     syscall_handler


syscall_handler:

    cmpl    $0, %eax
    jle     bad

    pushl   gs
    pushl   fs
    pushl   es
    pushl   ds

    pushl   %ebx
    pushl   %ecx
    pushl   %edx

    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %esp


bad:

    movl    $-1, %eax
    iret

done:

    popl    %esp
    popl    %esi
    popl    %edi 
    popl    %ebp

    popl    %edx
    popl    %ecx
    popl    %ebx

    popl    %ds
    popl    %es 
    popl    %fs 
    popl    %gs 

    iret 

systemcall_jmptable:

    .long   sys_halt
    .long   sys_execute
    .long   sys_read
    .long   sys_write
    .long   sys_open
    .long   sys_close

