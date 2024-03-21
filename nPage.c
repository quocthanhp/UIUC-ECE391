#include "nPage.h"

void pageInit()
{
    
    uint32_t i;

    //  setup the page table first
    for (i = 0; i < NUM_PTE; i++)
    {
        // flags for video memory
        if ((i >= VIDEO_MEM_INDEX) || (i <= VIDEO_MEMORY_DIRECT_INDEX))
        {
            page_table[i].isPresent = 1;
        } else {
            page_table[i].isPresent = 0;
        }
        
        page_table[i].isGlobal = 0;
        page_table[i].isDirty = 0;
        page_table[i].isReadWrite = 0;
        page_table[i].isAccessed = 0;

        page_table[i].isWriteThrough = 0;
        page_table[i].isCacheDisabled = 0;
        
        page_table[i].isAvailable = 0;
        page_table[i].isPat = 0;
        page_table[i].isUserSupervisor = 0;
        

        if ((i >= VIDEO_MEM_INDEX) || (i <= VIDEO_MEMORY_DIRECT_INDEX)) 
            page_table[i].pageBaseAddr = VIDEO_MEM_INDEX;
        else
            page_table[i].pageBaseAddr = i;
    }

    
    //first 4MB memory as 4kb
    page_directory[0].pde_KB.isGlobal = 0;
    page_directory[0].pde_KB.isAvailable = 0;

    
    page_directory[0].pde_KB.isCacheDisabled = 0;
    
    page_directory[0].pde_KB.page_size = 0;
    page_directory[0].pde_KB.isPresent = 1;

    page_directory[0].pde_KB.isReadWrite = 0;

    page_directory[0].pde_KB.isUserSupervisor = 0;
    page_directory[0].pde_KB.isWriteThrough = 0;

    page_directory[0].pde_KB.isAccessed = 0;
    page_directory[0].pde_KB.isReserved = 0;

    page_directory[0].pde_KB.pageTableBaseAddr = (unsigned int) page_table >> PAGE_TABLE_ADDR_OFFSET_4KB;


    // 4MB-8MB memory as 4mb pages
    page_directory[1].pde_MB.isCacheDisabled = 0;
    page_directory[1].pde_MB.isAccessed = 0;
    page_directory[1].pde_MB.isDirty = 0;

    
    page_directory[1].pde_MB.isUserSupervisor = 0;
    page_directory[1].pde_MB.isWriteThrough = 0;
    
    page_directory[1].pde_MB.page_size = 1;
    page_directory[1].pde_MB.isGlobal = 1;

    page_directory[1].pde_MB.isPat = 0;
    page_directory[1].pde_MB.isReserved = 0;

    page_directory[1].pde_MB.pageBaseAddr = 1;
    page_directory[1].pde_MB.isPresent = 1;
    page_directory[1].pde_MB.isReadWrite = 0;

    page_directory[1].pde_MB.isAvailable = 0;
    

    // Last part to init the remaining page directory 
    for (i = 2; i < NUM_PDE; i++)
    {   
        page_directory[i].pde_MB.isUserSupervisor = 0;
        page_directory[i].pde_MB.isWriteThrough = 0;
        page_directory[i].pde_MB.isCacheDisabled = 0;

        page_directory[i].pde_MB.page_size = 1;
        page_directory[i].pde_MB.isGlobal = 0;
        page_directory[i].pde_MB.isAvailable = 0;
        page_directory[i].pde_MB.isReserved = 0;
        page_directory[i].pde_MB.pageBaseAddr = i;

        page_directory[i].pde_MB.isPresent = 0;
        page_directory[i].pde_MB.isReadWrite = 0;
        
        page_directory[i].pde_MB.isAccessed = 0;
        page_directory[i].pde_MB.isDirty = 0;
        
        page_directory[i].pde_MB.isPat = 0;
        
    }

    enable_paging((unsigned int*) page_directory);
    return;
}