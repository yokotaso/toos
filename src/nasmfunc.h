#ifndef NSAM_FUNC_H
#define NASM_FUNC_H

void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void); 
void asm_inthandler27(void); 
void asm_inthandler2c(void); 
void io_sti_hlt(void);
#endif
