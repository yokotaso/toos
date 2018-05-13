#include <stdarg.h>
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5 
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8 
#define COL8_840000		9
#define COL8_008400		10 
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15


struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    unsigned char *vram;
};

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palatte(void);
void set_pallate(int start, int end, unsigned char *rgb);

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(unsigned char *vram, int x, int y);

void putfont8_asc(unsigned char *vram, int xsize, int x, int y, char c, char *s);
void putfont8(unsigned char *vram, int xsize, int x, int y,char c,char *font);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(unsigned char *vram, int vxsize, int pxsize, int pysize, int px, int py, char *buf, int bxsize);
int sprintf(char *copy, char *format, ...);

void HariMain(void) {
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0xff0;
    
    init_palatte();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    putfont8_asc(binfo->vram, binfo->scrnx, 30, 30, COL8_000000, "Haribote OS.");
    putfont8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_FFFFFF, "Haribote OS.");

    char s[64], mcursor[256];
    sprintf(s, "scrnx: %d / scrny: %d", binfo->scrnx, binfo->scrny);
    putfont8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

    init_mouse_cursor8(mcursor, COL8_008484);

    int mx = (binfo->scrnx - 16) / 2; /* 画面中央になるように座標計算 */
    int my = (binfo->scrny - 28 - 16) / 2;
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);    
    for(;;) {
        io_hlt();
    }
}

void init_screen8(unsigned char *vram, int x, int y) {
    boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
    boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
    boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
    boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);
    
    boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
    boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
    boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
    boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
    boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
    boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);
    
    boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
    boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
    boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
    boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
    return;
}

void putfont8(unsigned char *vram, int xsize, int x, int y,char c,char *font) {
    char d;
    for(int i = 0; i < 16; i++) {
        d = font[i];
        if((d & 0x80) != 0) {
            vram[(y + i) * xsize + x + 0] = c;
        }
        if((d & 0x40) != 0) {
            vram[(y + i) * xsize + x + 1] = c;
        }
        if((d & 0x20) != 0) {
            vram[(y + i) * xsize + x + 2] = c;
        }
        if((d & 0x10) != 0) {
            vram[(y + i) * xsize + x + 3] = c;
        }
        if((d & 0x08) != 0) {
            vram[(y + i) * xsize + x + 4] = c;
        }
        if((d & 0x04) != 0) {
            vram[(y + i) * xsize + x + 5] = c;
        }
        if((d & 0x02) != 0) {
            vram[(y + i) * xsize + x + 6] = c;
        }
        if((d & 0x01) != 0) {
            vram[(y + i) * xsize + x + 7] = c;
        }

    }
}

void putfont8_asc(unsigned char *vram, int xsize, int x, int y, char c, char *s) {
    extern char hankaku[];
    for(; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;
    }
}

void init_palatte(void) {
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,	/*  0:黒 */
        0xff, 0x00, 0x00,	/*  1:明るい赤 */
        0x00, 0xff, 0x00,	/*  2:明るい緑 */
        0xff, 0xff, 0x00,	/*  3:明るい黄色 */
        0x00, 0x00, 0xff,	/*  4:明るい青 */
        0xff, 0x00, 0xff,	/*  5:明るい紫 */
        0x00, 0xff, 0xff,	/*  6:明るい水色 */
        0xff, 0xff, 0xff,	/*  7:白 */
        0xc6, 0xc6, 0xc6,	/*  8:明るい灰色 */
        0x84, 0x00, 0x00,	/*  9:暗い赤 */
        0x00, 0x84, 0x00,	/* 10:暗い緑 */
        0x84, 0x84, 0x00,	/* 11:暗い黄色 */
        0x00, 0x00, 0x84,	/* 12:暗い青 */
        0x84, 0x00, 0x84,	/* 13:暗い紫 */
        0x00, 0x84, 0x84,	/* 14:暗い水色 */
        0x84, 0x84, 0x84	/* 15:暗い灰色 */
    };
    set_pallate(0, 15, table_rgb);
}

void set_pallate(int start, int end, unsigned char *rgb) {
    int eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for(int i = 0;i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1) {
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
    return;
}


void init_mouse_cursor8(char *mouse, char bc) {
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
    };
    
    for(int y = 0; y < 16; y++) {
        for(int x = 0; x < 16; x++) {
            if(cursor[y][x] == '*') {
               mouse[y * 16 + x] = COL8_000000;
            }

            if(cursor[y][x] == 'O') {
               mouse[y * 16 + x] = COL8_FFFFFF;
            }

            if(cursor[y][x] == '.') {
               mouse[y * 16 + x] = bc;
            }
        }
    }
}
 
void putblock8_8(unsigned char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize) {
    int x,y;
    for(y = 0; y < pysize; y++) {
        for(x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 +x)] = buf[y * bxsize + x];
        }
    }
}

int dec_to_ascii(char *str, int dec) {

   int max_digits = 1;
   int digits = 1;
   while(dec / max_digits > 9) {
       digits += 1;
       max_digits *= 10; 
   }
   
   for(int i = 0; i < 3; i++) {
       int number_at_n_digits = dec / max_digits;

       str[i] = 0x30 + number_at_n_digits; 
       dec -= number_at_n_digits * max_digits;
       max_digits = max_digits / 10;
   }

   return 3;
}

int sprintf(char *copy, char *format, ...) {
    va_list list;
    va_start(list, format);
    int i = 0, j = 0;
    while(format[i] != '\0') {
        if(format[i] == '%') {
           i++;
 
           if(format[i] == 'd') {
               int number = va_arg(list, int);
               int digits = dec_to_ascii(&copy[j], number);
               // copy[j] なので digits進める
               // format[i] は dの1文字分
               i++;
               j += digits;
           } 

        } else {
           copy[j] = format[i];

           i++;
           j++;
        }
    }

    va_end(list);
    copy[j++] = '\0';
    return 0;
}

