#define ASM     1

/*taken from osdev enabling paging document*/
.text

#define ENABLE_PAGING   \
    pushl %ebp 
    movl %esp. %ebp 
    movl 8(%esp), %esi
    movl %esi, %cr3 ;\

    movl %cr0, %eax            ;\
    orl  0x8000000, %eax     ;\
    movl %cr0, %eax            ;\
    movl %cr4, %eax            ;\
    orl 0x00000010, %eax      ;\
    movl %eax, %cr4            ;\

.end