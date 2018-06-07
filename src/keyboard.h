#ifndef KEYBOARD_H
#define KEYAORRD_H

#include "fifo.h"
#include "nasmfunc.h"
#include "int.h"

#define PORT_KEYSTA                0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE        0x60
#define KBC_MODE                0x47
#define PORT_KEYDAT                0x0060
#define PORT_KEYCMD                0x0064

#define KEYCMD_SENDTO_MOUSE        0xd4
#define MOUSECMD_ENABLE            0xf4

void init_keyboard(void);
void inthandler21(int *esp);
void wait_KBC_sendready(void);
#endif
