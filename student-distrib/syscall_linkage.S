#define ASM 1
#include "x86_desc.h"
.GLOBAL     syscall_handler


syscall_handler:

    #pushl   gs
    #pushl   fs
    #pushl   es
    #pushl   ds

    
    pushfl
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %esp
    pushl   %ebx
    pushl   %ecx
    pushl   %edx

    cmpl    $0, %eax
    jle     bad

    cmpl    $10, %eax
    ja      bad

    call *systemcall_jmptable(, %eax, 4)
    jmp done

bad:

    movl    $-1, %eax
    #iret

done:

    popl    %edx
    popl    %ecx
    popl    %ebx
    popl    %esp
    popl    %esi
    popl    %edi 
    popl    %ebp
    popfl
  
    #popl    %ds
    #popl    %es 
    #popl    %fs 
    #popl    %gs 

    iret 

systemcall_jmptable:

    .long   sys_halt
    .long   sys_execute
    .long   sys_read
    .long   sys_write
    .long   sys_open
    .long   sys_close

