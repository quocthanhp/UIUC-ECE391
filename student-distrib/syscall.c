#include "syscall.h"
#include "x86_desc.h"
#include "fs.h"



uint32_t pid = -1;
extern void flush_tlb();


uint32_t get_next_pid() {
    return ++pid;
}

void set_program_page(uint32_t pid) {
    uint32_t pde_id = PROGRAM_VIRTUAL >> PAGE_DIR_OFFSET;
    
    page_directory[pde_id].pde_MB.isPresent = 1; 
    page_directory[pde_id].pde_MB.isPageSize = 1; /* 4MB page */  
    page_directory[pde_id].pde_MB.isGlobal = 0; /* page is a per-process page and the translations will be cleared when CR3 is reloaded*/ 
    page_directory[pde_id].pde_MB.isUserSupervisor = 1; /* user-level */ 
    page_directory[pde_id].pde_MB.pageBaseAddr = (PROGRAM_PHYSICAL + (pid * PROGRAM_SPACE)) >> PAGE_FRAME_OFFSET;

    flush_tlb();
}


int32_t sys_halt (uint8_t status){}
int32_t sys_execute (const uint8_t* command){}
int32_t sys_read (int32_t fd, void* buf, int32_t nbytes){}
int32_t sys_write (int32_t fd, const void* buf, int32_t nbytes){}
int32_t sys_open (const uint8_t* filename){}
int32_t sys_close (int32_t fd){}
