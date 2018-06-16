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


void memman_init(struct MEMMAN *man) {
    man->frees = 0;
    man->maxfrees = 0;
    man->lostsize = 0;
    man->losts = 0;
    return;
}

unsigned int memman_total(struct MEMMAN *man) {
    unsigned int t = 0;
    for(int i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }
    return t;
}

unsigned int memman_alloc(struct MEMMAN *man, unsigned int size) {
    unsigned a;
    for(int i = 0; i < man->frees; i++) {
        if(man->free[i].size >= size) {
            a = man->free[i].addr;
            man->free[i].addr += size;
            man->free[i].size -= size; 
            if(man->free[i].size == 0) {
                man->frees--;
                for(; i < man->frees; i++) {
                    man->free[i] = man->free[i + 1]; 
                }
            }
            return a;
        }
    }
    return 0;
}

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size) {
   int p = 0;
   for(int i = 0; i < man->frees; i++, p++) {
       if(man->free[i].addr > addr) {
           break;
       }
   }

   if(p > 0) {
      // merge previous memory frees
      if(man->free[p - 1].addr + man->free[p -1].size == addr) {
          man->free[p - 1].size += size;
          if( p < man->frees ) {
              man->free[p -1].size += size; 
              // merge backward memofy frees
              if( p < man->frees) {
                  if(addr + size == man->free[p].addr) {
                      man->free[p -1].size += man->free[p].size;
                      man->frees--;
                      for(;p < man->frees; p++) {
                          man->free[p] = man->free[p + 1];
                      }
                  }
              }
              return 0;
          }
      }
   }

  if(p < man->frees) {
    // merge backward memory frees
    if(addr + size == man->free[p].addr) {
        man->free[p].addr = addr;
        man->free[p].size += size;
        return 0;
    }
  }

      // Can not merge previous or backward memory frees
  if(man->frees < MEMMAN_FREES) {
    // move all memory free to backward.
    for(int j = man->frees; j > p; j--) {
        man->free[j] = man->free[j - 1];
    }
    man->frees++;
    if(man->maxfrees < man->frees) {
        man->maxfrees = man->frees;
    }
    man->free[p].addr = addr;
    man->free[p].size = size;
    return 0;
  }

   man->losts++;
   man->lostsize += size;
   return -1;
}

