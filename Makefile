ipl.bin: src/ipl.nasm
	nasm src/ipl.nasm -o ipl.bin -l ipl.lst

asmhead.bin: src/asmhead.nasm
	nasm src/asmhead.nasm -o asmhead.bin -l asmhead.lst
cursor.o: tools/resource-to-c resource/cursor.txt
	(cd tools/resource-to-c;go build .)
	tools/resource-to-c/resource-to-c -v cursor -f resource/cursor.txt -o cursor.c
	cc -march=i486 -m32 -nostdlib -c cursor.c 

font.o: tools/char-to-c resource/hankaku.txt
	(cd tools/char-to-c;go build .)
	tools/char-to-c/char-to-c -f resource/hankaku.txt -o font.c
	cc -march=i486 -m32 -nostdlib -c font.c

haribote.sys: asmhead.bin font.o cursor.o
	nasm -g -f elf32 src/nasmfunc.nasm -o nasmfunc.o -l nasmfunc.lst
	cc -march=i486 -m32 -nostdlib -c src/bootpack.c -Wall 
	ld -m elf_i386 -T os.ld bootpack.o nasmfunc.o font.o cursor.o -o bootpack.hrb
	cat asmhead.bin bootpack.hrb > haribote.sys

hello.img: ipl.bin haribote.sys Makefile
	mformat -f 1440 -B ipl.bin -C -i hello.img ::	
	mcopy haribote.sys -i hello.img ::

run: hello.img
	(set -eux;qemu-system-i386 -drive file=hello.img,format=raw,index=0,if=floppy)

clean:
	rm -fv hello.img *.bin *.sys *.lst *.o *.hrb *.c tools/char-to-c/char-to-c tools/resource-to-c/resource-to-c

setup:
	sudo apt-get install -y nasm qemu mtools golang-go libc6-dev-i386

