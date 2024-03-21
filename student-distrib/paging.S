#define ASM     1

.global enable_paging

.text

enable_paging:          
    pushl %ebp                 
    movl %esp, %ebp            
    movl 8(%esp), %esi         
    movl %esi, %cr3            
    movl %cr0, %eax            
    orl  $0x8000000, %eax       
    movl %eax, %cr0         
    movl %cr4, %eax            
    orl $0x00000010, %eax       
    movl %eax, %cr4                    


