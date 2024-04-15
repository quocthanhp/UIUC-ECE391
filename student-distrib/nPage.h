#ifndef _PAGING_H
#define _PAGING_H

#include "lib.h"
#include "types.h"

#define VIDEO_MEMORY_INDEX  0xB8000 // video memory index start

#define VIDEO_MEMORY_INDEX_END 0xB9000 //video memory index end

#define KERNEL_BASE_ADDRESS 0x400000

#define PAGE_TABLE_INDEX 0x003FF000 // used to extract the 21 - 12 bits of the address

#define PAGE_TABLE_OFFSET  12 //bits needed to shift for page directory's page table base address

#define PAGE_DIRECTORY_INDEX_OFFSET  22  //bits needed to shift to obtain the page directory's index


// this is to call initialization of page
void pageInit();
void enable_paging(unsigned int * page_directory);
#endif
