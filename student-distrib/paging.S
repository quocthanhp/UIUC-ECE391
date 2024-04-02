#define ASM     1

.global enable_paging, load_paging, flush_tlb

.text

load_paging:

    movl %eax, %cr3 
    ret

enable_paging:            

    pushl %ebp                 
    movl %esp, %ebp
    movl 8(%esp), %ecx

    movl %cr4, %eax            
    orl  $0x00000010, %eax       
    movl %eax, %cr4       

    movl %ecx, %cr3

    movl %cr0, %eax            
    orl  $0x80000000, %eax       
    movl %eax, %cr0    

    leave 
    ret

flush_tlb:
    movl %cr3, %eax
	movl %eax, %cr3
    ret
    
