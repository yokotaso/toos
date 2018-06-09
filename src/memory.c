#include "memory.h"

unsigned int memtest_sub(unsigned int start, unsigned int end) {
    unsigned int *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
    unsigned mem_position = start;
    for(unsigned int i = start; i <= end; i+=4, mem_position = i) {
        p = (unsigned int *)i;
        old = *p;
        *p = pat0;
        *p ^= 0xffffffff;
        if(*p != pat1) {
not_memory:
            *p = old;
            break;
        }
     
        *p ^= 0xffffffff;
        if(*p != pat0) {
            goto not_memory;
        }
        *p = old;
    }
    return mem_position; 
}

unsigned int memtest(unsigned int start, unsigned int end) {
    char flg486;
    unsigned int eflg, cr0, i;


    eflg = io_load_eflags();
    eflg |= EFLAGS_AC_BIT;
  
    io_store_eflags(eflg);
    eflg = io_load_eflags();
 
    if((eflg & EFLAGS_AC_BIT) != 0) {
        flg486 = 1;
    }

    eflg &= ~EFLAGS_AC_BIT;
    io_store_eflags(eflg);
 
    if(flg486 != 0) {
        cr0 = load_cr0();
        cr0 |= CR0_CACHE_DISABLE;
        store_cr0(cr0);
    }
 
    i = memtest_sub(start, end);
    
    if(flg486 != 0) {
        cr0 = load_cr0();
        cr0 &= ~CR0_CACHE_DISABLE;
        store_cr0(cr0);
    }
   
    return i;
}


