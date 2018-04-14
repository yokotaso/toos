ipl.bin: src/ipl.nasm Makefile
	nasm src/ipl.nasm -o ipl.bin -l ipl.lst

footer.bin: src/footer.nasm Makefile
	nasm src/footer.nasm -o footer.bin -l footer.lst

image: ipl.bin footer.bin Makefile
	cat ipl.bin footer.bin > hello.img

run: hello.img
	(set -eux;qemu-system-i386 -drive file=hello.img,format=raw)

clean:
	rm -fv hello.img ipl.* footer.*

setup:
	sudo apt-get install -y nasm qemu

