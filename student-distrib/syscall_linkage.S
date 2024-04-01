#define ASM 1
#include "x86_desc.h"

.data
    MIN_SYS_NUM = 1
    MAX_SYS_NUM = 6

.GLOBAL     syscall_handler

syscall_handler:
    pushfl
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %esp
    
    #Push arguments to handler
    pushl   %edx
    pushl   %ecx
    pushl   %ebx

    cmpl    $MIN_SYS_NUM, %eax
    jb      bad

    cmpl    $MAX_SYS_NUM, %eax
    ja      bad

    call *systemcall_jmptable(, %eax, 4)
    jmp done

bad:
    movl    $-1, %eax

done:
    popl    %ebx
    popl    %ecx
    popl    %edx
    popl    %esp
    popl    %esi
    popl    %edi 
    popl    %ebp
    popfl
  
    iret 

systemcall_jmptable:
    .long   0x00
    .long   halt
    .long   execute
    .long   read
    .long   write
    .long   open
    .long   close

