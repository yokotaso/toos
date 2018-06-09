#include "asmhead.h"
#include "graphic.h" 
#include "dsctbl.h"
#include "nasmfunc.h"
#include "mystdio.h"
#include "int.h"
#include "fifo.h"
#include "mouse.h"
#include "keyboard.h"
extern struct FIFO8 keyinfo;
extern struct FIFO8 mouseinfo;

#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000
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

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0xff0;

    init_gdtidt();
    init_pic();
    io_sti();

    io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
    io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

    
    init_keyboard();    
    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    char mcursor[256];
    init_mouse_cursor8(mcursor, COL8_008484);

    int mx = (binfo->scrnx - 16) / 2; /* 画面中央になるように座標計算 */
    int my = (binfo->scrny - 28 - 16) / 2;

    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
    struct MOUSE_DEC mdec;
    enable_mouse(&mdec);

    unsigned char keybuf[32], mousebuf[128]; 
    fifo8_init(&keyinfo, 32 ,keybuf);
    fifo8_init(&mouseinfo, 128 ,mousebuf);
    
    char memory_test[32];
    int num_of_memory = memtest(0x00400000, 0xbfffffff) / (1024 * 1024);
    sprintf(memory_test, "memory %dMB", num_of_memory);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 32, COL8_FFFFFF, memory_test);

    for (;;) {
        io_cli();
        if (fifo8_status(&keyinfo) + fifo8_status(&mouseinfo) == 0) {
            io_sti_hlt();
        } else {
            if(fifo8_status(&keyinfo) != 0) {
                int i = fifo8_get(&keyinfo); 
                io_sti();
                char s[30];
                sprintf(s, "%02X", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            } else if(fifo8_status(&mouseinfo) != 0) {
                int i = fifo8_get(&mouseinfo); 
                io_sti();
                if (mouse_decode(&mdec, i) != 0) {
                    /* データが3バイト揃ったので表示 */
                    char s[30];
                    sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
                    if((mdec.btn & 0x01) != 0) {
                        s[1] = 'L';
                    }
                    if((mdec.btn & 0x02) != 0) {
                        s[3] = 'R';
                    }
                    if((mdec.btn & 0x04) != 0) {
                        s[2] = 'C';
                    }

                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 8 * 15 - 1, 31);
                    putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15);
                    mx += mdec.x;
                    my += mdec.y;

                    if(mx < 0) {
                        mx = 0;
                    }

                    if(my < 0) {
                        my = 0;
                    }

                    if(mx > binfo->scrnx - 16) {
                        mx = binfo->scrnx -16;
                    }

                    if(my > binfo->scrny - 16) {
                        my = binfo->scrny -16;
                    }
                    char cursor_position[64];
                    sprintf(cursor_position, "(%3d, %3d)", mx, my);
                    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15);
                    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, cursor_position);
                    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
                }
            }
        }
    }
}


