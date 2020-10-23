NASMFLAGS = -i boot/
CC = gcc
CCFLAGS = -c -m32 -fno-pie -fno-asynchronous-unwind-tables -fno-stack-protector -Wall
LDFLAGS = -melf_i386 -T kernel/kernel.lds

start_bochs: hd.img
	bochs -f bochs/bochsrc
	
hd.img: mbr.bin loader.bin kernel.bin
	dd if=tmp/mbr.bin of=bochs/hd.img bs=512 count=1 conv=notrunc
	dd if=tmp/loader.bin of=bochs/hd.img bs=512 seek=1 conv=notrunc
	dd if=tmp/kernel.bin of=bochs/hd.img bs=512 seek=5 conv=notrunc
	
mbr.bin: boot/mbr.s
	nasm $(NASMFLAGS) -o tmp/mbr.bin boot/mbr.s
	
loader.bin: boot/loader.s
	nasm $(NASMFLAGS) -o tmp/loader.bin boot/loader.s

kernel.bin: kernel.elf
	objcopy -O binary -R .note.gnu.property -R .comment tmp/kernel.elf tmp/kernel.bin

kernel.elf: kernel.o hardware.o init.o display.o common.o gdt.o idt.o keyboard.o timer.o task.o
	$(LD) $(LDFLAGS) -o tmp/kernel.elf tmp/kernel.o tmp/hardware.o tmp/init.o tmp/display.o tmp/common.o tmp/gdt.o tmp/idt.o tmp/keyboard.o tmp/timer.o tmp/task.o
	objdump -M intel -D tmp/kernel.elf > tmp/kernel.dump

kernel.o: kernel/kernel.c
	$(CC) $(CCFLAGS) -o tmp/kernel.o kernel/kernel.c

hardware.o: kernel/hardware.s
	nasm -f elf32 -o tmp/hardware.o kernel/hardware.s

init.o: kernel/init.c
	$(CC) $(CCFLAGS) -o tmp/init.o kernel/init.c

display.o: kernel/display.c
	$(CC) $(CCFLAGS) -o tmp/display.o kernel/display.c
	
common.o: kernel/common.c
	$(CC) $(CCFLAGS) -o tmp/common.o kernel/common.c

gdt.o: kernel/gdt.c
	$(CC) $(CCFLAGS) -o tmp/gdt.o kernel/gdt.c

idt.o: kernel/idt.c
	$(CC) $(CCFLAGS) -o tmp/idt.o kernel/idt.c

keyboard.o: kernel/keyboard.c
	$(CC) $(CCFLAGS) -o tmp/keyboard.o kernel/keyboard.c
	
timer.o: kernel/timer.c
	$(CC) $(CCFLAGS) -o tmp/timer.o kernel/timer.c
	
task.o: kernel/task.c
	$(CC) $(CCFLAGS) -o tmp/task.o kernel/task.c

	
clean:
	rm -f tmp/*.*
