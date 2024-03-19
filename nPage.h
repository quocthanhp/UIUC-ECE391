#ifndef _PAGING_H
#define _PAGING_H

#include "lib/lib.h"
#include "lib/types.h"
#include "x86_desc.h"


// Table address offset for 4KB pages
#define PAGE_TABLE_ADDR_OFFSET_4KB    12
// Table address offset for 4MB pages
#define PAGE_TABLE_ADDR_OFFSET_4MB    22

// video memory index 
#define VIDEO_MEMORY_INDEX  0xB8000 // 0xb0  //needs to be changed
// direct access Index to video memory
#define VIDEO_MEMORY_DIRECT_INDEX 0xB9000 // 0xb1 //needs to be changed


// this is to call initialization of page
void pageInit();

#endif