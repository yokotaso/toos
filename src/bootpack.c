void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
void io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palatte(void);
void set_pallate(int start, int end, unsigned char *rgb);

void HariMain(void) {
    // init_palatte();
    char *p = (char *) 0xa0000;
 
    for(int i = 0; i < 0xffff; i++ ) {
        p[i] = i & 0x0f;
    }
 
    for(;;) {
        io_hlt();
    }
}



