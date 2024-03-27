#ifndef _PAGING_H
#define _PAGING_H

#include "lib.h"
#include "types.h"


// video memory index 
#define VIDEO_MEMORY_INDEX  0xB8000 // 0xb0  //needs to be changed
// direct access Index to video memory
#define VIDEO_MEMORY_DIRECT_INDEX 0xB9000 // 0xb1 //needs to be changed

// this is to call initialization of page
void pageInit();
void enable_paging(unsigned int * page_directory);
#endif
