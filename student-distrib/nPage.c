
#include "nPage.h"

#include "x86_desc.h"


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
            page_table[i].pageBaseAddr = (VIDEO_MEMORY_INDEX >> 12);
        } 
    }

    
    //first 4MB memory as 4kb
    page_directory[0].pde_KB.isPresent = 1;

    page_directory[0].pde_KB.isReadWrite = 1;

    page_directory[0].pde_KB.pageTableBaseAddr = (unsigned int) page_table >> 12;


    // 4MB-8MB memory as 4mb pages
    page_directory[1].pde_MB.isPageSize = 1;
    page_directory[1].pde_MB.pageBaseAddr = 0x400000 >> 22;
    page_directory[1].pde_MB.isPresent = 1;
    page_directory[1].pde_MB.isReadWrite = 1;
    page_directory[1].pde_MB.isGlobal = 0;
    page_directory[1].pde_MB.isUserSupervisor = 0;

    //enable paging by modifying the values within the correct registers
    enable_paging((unsigned int *) page_directory);
    return;
}
