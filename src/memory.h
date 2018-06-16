#ifndef MEMORY_H
#define MEMORY_H

#include "nasmfunc.h"
#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000
#define MEMMAN_FREES 4090

struct FREEINFO {
    unsigned int addr, size;
};

struct MEMMAN {
    int frees, maxfrees, lostsize, losts;
    struct FREEINFO free[MEMMAN_FREES];
};

unsigned int memtest_sub(unsigned int start, unsigned int end);
unsigned int memtest(unsigned int start, unsigned int end);

void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *main, unsigned int size);
int memman_free(struct MEMMAN *main, unsigned int addr, unsigned int size);
#endif
