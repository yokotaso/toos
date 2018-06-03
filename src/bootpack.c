#include "asmhead.h"
#include "graphic.h" 
#include "dsctbl.h"
#include "nasmfunc.h"
#include "io.h"
#include "int.h"
#include "fifo.h"

struct FIFO8 keyinfo;
struct FIFO8 mouseinfo;

void enable_mouse(void);
void init_keyboard(void);

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
    char s[64];
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

    enable_mouse();

    unsigned char keybuf[32], mousebuf[128]; 
    fifo8_init(&keyinfo, 32 ,keybuf);
    fifo8_init(&mouseinfo, 128 ,mousebuf);

    for (;;) {
        io_cli();
        if (fifo8_status(&keyinfo) + fifo8_status(&mouseinfo) == 0) {
            io_sti_hlt();
        } else {
            if(fifo8_status(&keyinfo) != 0) {
                int i = fifo8_get(&keyinfo); 
                io_sti();
                char s[30];
                sprintf(s, "%X", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            } else if(fifo8_status(&mouseinfo) != 0) {
                int i = fifo8_get(&mouseinfo); 
                io_sti();
                char s[30];
                sprintf(s, "%X", i);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 47, 31);
                putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);

            }
        }
    }
}


#define PORT_KEYDAT                0x0060
#define PORT_KEYSTA                0x0064
#define PORT_KEYCMD                0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE        0x60
#define KBC_MODE                0x47

void wait_KBC_sendready(void) {
    /* キーボードコントローラがデータ送信可能になるのを待つ */
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void) {
    /* キーボードコントローラの初期化 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

#define KEYCMD_SENDTO_MOUSE        0xd4
#define MOUSECMD_ENABLE            0xf4

void enable_mouse(void) {
    /* マウス有効 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return; /* うまくいくとACK(0xfa)が送信されてくる */
}

