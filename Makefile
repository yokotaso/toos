ipl.bin: src/ipl.nasm Makefile
	nasm src/ipl.nasm -o ipl.bin -l ipl.lst


asmhead.bin: src/asmhead.nasm Makefile
	nasm src/asmhead.nasm -o asmhead.bin -l asmhead.lst

nasmfunc.o: src/nasmfunc.nasm
	nasm -g -f elf src/nasmfunc.nasm -o nasmfunc.o -l nasmfunc.lst

haribote.sys: asmhead.bin nasmfunc.o
	gcc -march=i486 -m32 -nostdlib -T os.ld src/bootpack.c nasmfunc.o -o bootpack.hrb -Wall 
	cat asmhead.bin bootpack.hrb > haribote.sys

hello.img: ipl.bin haribote.sys Makefile
	mformat -f 1440 -B ipl.bin -C -i hello.img ::	
	mcopy haribote.sys -i hello.img ::

run: hello.img
	(set -eux;qemu-system-i386 -drive file=hello.img,format=raw,index=0,if=floppy)

clean:
	rm -fv hello.img *.bin *.sys *.lst *.o *.hrb

setup:
	sudo apt-get install -y nasm qemu mtools

