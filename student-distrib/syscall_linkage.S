#define ASM 1
#include "x86_desc.h"

.data
    MIN_SYS_NUM = 1
    MAX_SYS_NUM = 6

.GLOBAL     syscall_handler

syscall_handler:

    # save all regs 
    pushfl
    pushl   %ebx
    pushl   %ecx
    pushl   %edx
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %esp
    
    # Push arguments to handler
    pushl   %edx
    pushl   %ecx
    pushl   %ebx


    # check system call number
    cmpl    $MIN_SYS_NUM, %eax
    jb      bad
    cmpl    $MAX_SYS_NUM, %eax
    ja      bad

    call *systemcall_jmptable(, %eax, 4)

    #addl     $12, %esp     
    jmp     done

bad:
    movl    $-1, %eax

done:
    popl    %esp
    popl    %esi
    popl    %edi 
    popl    %ebp
    popl    %edx
    popl    %ecx
    popl    %ebx
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

