#define ASM     1

.global enable_paging

.text

enable_paging:          
    pushl %ebp                 
    movl %esp, %ebp            
    movl 8(%esp), %eax         
    movl %eax, %cr3 

    movl %cr0, %eax            
    orl  $0x80000001, %eax       
    movl %eax, %cr0    
         
    movl %cr4, %eax            
    orl  $0x00000010, %eax       
    movl %eax, %cr4                    


