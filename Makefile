ipl.bin: src/ipl.nasm Makefile
	nasm src/ipl.nasm -o ipl.bin -l ipl.lst

haribote.sys: src/haribote.nasm Makefile
	nasm src/haribote.nasm -o haribote.sys -l haribote.lst

hello.img: ipl.bin haribote.sys Makefile
	mformat -f 1440 -B ipl.bin -C -i hello.img ::	
	mcopy haribote.sys -i hello.img ::

run: hello.img
	(set -eux;qemu-system-i386 -drive file=hello.img,format=raw,index=0,if=floppy)

clean:
	rm -fv hello.img *.bin *.sys

setup:
	sudo apt-get install -y nasm qemu mtools

