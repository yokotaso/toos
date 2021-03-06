#ifndef ASMHEAD_H 
#define ASMHEAD_H 

struct BOOTINFO { /* 0x0ff0-0x0fff */
    char cyls; /* ブートセクタはどこまでディスクを読んだのか */
    char leds; /* ブート時のキーボードのLEDの状態 */
    char vmode; /* ビデオモード  何ビットカラーか */
    char reserve;
    short scrnx, scrny; /* 画面解像度 */
    char *vram;
};
#define ADR_BOOTINFO    0x00000ff0

#endif
