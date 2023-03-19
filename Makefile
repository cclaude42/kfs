###################################
# KFS
###################################

NAME = kfs.iso
BIN = kfs.bin

AC = i386-elf-as
AFLAGS = 
ASRC = boot/boot.S

LC = i386-elf-gcc
LFLAGS = -ffreestanding -O2 -nostdlib -lgcc
LSRC = boot/linker.ld

NC = nasm
NFLAGS = -f elf
NSRC = src/flush.s

CC = i386-elf-gcc
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
CSRC = src/kernel.c src/print.c src/gdt.c src/idt.c src/pic.c

OBJ = $(ASRC:.S=.o) $(NSRC:.s=.o) $(CSRC:.c=.o)

ISODIR = isodir
CFG = boot/grub.cfg

###################################
# Methods
###################################

all: $(NAME)

.S.o:
	$(AC) $< -o $@

.s.o:
	$(NC) $(NFLAGS) $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(BIN)
	mkdir -p $(ISODIR)/boot/grub
	cp $(BIN) $(ISODIR)/boot/$(BIN)
	cp $(CFG) $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(NAME) $(ISODIR)

$(BIN): $(OBJ)
	$(CC) -T $(LSRC) -o $(BIN) $(OBJ) $(LFLAGS)

run: re
	qemu-system-i386 -cdrom $(NAME)

clean:
	rm -f $(OBJ)
	rm -f $(BIN)
	rm -rf $(ISODIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re run
