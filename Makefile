###################################
# KFS
###################################

NAME = kfs

###################################
# Methods
###################################

all: $(NAME)

$(NAME):
	i386-elf-as boot.s -o boot.o
	i386-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i386-elf-gcc -T linker.ld -o kfs.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	mkdir -p isodir/boot/grub
	cp kfs.bin isodir/boot/kfs.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kfs.iso isodir

run:
	qemu-system-i386 -cdrom kfs.iso

clean:
	rm -f boot.o
	rm -f kernel.o
	rm -f kfs.bin
	rm -rf isodir

fclean: clean
	rm -f kfs.iso

re: fclean all

.PHONY: clean fclean re run