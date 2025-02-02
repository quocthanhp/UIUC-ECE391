
#include "nPage.h"

#include "x86_desc.h"

#include "terminal.h"

extern void flush_tlb();

extern terminal_t terminals[3]; // make array of 3
extern int active_terminal;
/*
 * page INIT
 * sets up the paging structures (page table and page directory)
 * structs for the page table and page directory defined in 0x86_desc.h
 * calls enable paging
 * INPUTS:NONE
 * OUTPUTS:NONE
 * SIDE EFFECTS: enables paging to be used by the OS
 * 
 */
void pageInit()
{
    
    uint32_t i;

    //  setup the page table first
    for (i = 0; i < NUM_PTE; i++)
    {
        page_table[i].value = 0;
        page_directory[i].value = 0;
        // flags for video memory
        if ((i == (VIDEO_MEMORY_INDEX >> 12)))
        {
            page_table[i].isPresent = 1;
            page_table[i].isReadWrite = 1;
            page_table[i].pageBaseAddr = (VIDEO_MEMORY_INDEX >> PAGE_TABLE_OFFSET);
        } 
    }

    
    //first 4MB memory as 4kb
    page_directory[0].pde_KB.isPresent = 1;

    page_directory[0].pde_KB.isReadWrite = 1;

    page_directory[0].pde_KB.pageTableBaseAddr = (unsigned int) page_table >> PAGE_TABLE_OFFSET;


    // 4MB-8MB memory as 4mb pages
    page_directory[1].pde_MB.isPageSize = 1;
    page_directory[1].pde_MB.pageBaseAddr = KERNEL_BASE_ADDRESS >> PAGE_DIRECTORY_INDEX_OFFSET;
    page_directory[1].pde_MB.isPresent = 1;
    page_directory[1].pde_MB.isReadWrite = 1;
    page_directory[1].pde_MB.isGlobal = 0;
    page_directory[1].pde_MB.isUserSupervisor = 0;

    //enable paging by modifying the values within the correct registers
    enable_paging((unsigned int *) page_directory);
    return;
}

/*
 * terminal_page_int
 * sets up the paging structures (page table and page directory)
 * for the video memory contents of each terminal
 * INPUTS:index of the terminal whose video memory to be initialized
 * OUTPUTS:NONE
 * SIDE EFFECTS: enables paging at the desired memory indices
 */
void terminal_page_init(int terminal_index)
{   
    uint32_t pdeid = (terminals[terminal_index].video_memory >> PAGE_DIRECTORY_INDEX_OFFSET);
    uint32_t pteid = ( (terminals[terminal_index].video_memory & PAGE_TABLE_INDEX) >> PAGE_TABLE_OFFSET);

    page_directory[pdeid].pde_KB.isPageSize = 0; 
    page_directory[pdeid].pde_KB.isPresent = 1;
    page_directory[pdeid].pde_KB.isUserSupervisor = 1;
    page_directory[pdeid].pde_KB.isReadWrite = 1;
    page_directory[pdeid].pde_KB.pageTableBaseAddr = ((uint32_t) page_table >> PAGE_TABLE_OFFSET);
    
    page_table[pteid].isPresent = 1;
    page_table[pteid].isReadWrite = 1;
    page_table[pteid].isUserSupervisor = 1;
    page_table[pteid].pageBaseAddr = ((terminals[terminal_index].video_memory) >> PAGE_TABLE_OFFSET);

    flush_tlb();
}
