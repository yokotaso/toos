#include "keyboard.h"
struct FIFO8 keyinfo;

/* PS/2キーボードからの割り込み */
void inthandler21(int *esp)
{
    io_out8(PIC0_OCW2, 0x61);
    int data = io_in8(PORT_KEYDAT);
    fifo8_put(&keyinfo, data);
    return;
}


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

