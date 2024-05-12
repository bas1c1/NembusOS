nasm -f elf32 src/kernel/boot.asm -o build/boot.o 
gcc -c -m32 src/kernel/kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -c -m32 src/impl/memory.c -o build/memory.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/vga.c -o build/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/std.c -o build/std.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/string.c -o build/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/io.c -o build/io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/cmos.c -o build/cmos.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/keyboard.c -o build/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/gdt.c -o build/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/isr.c -o build/isr.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/timer.c -o build/timer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/paging.c -o build/paging.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/kheap.c -o build/kheap.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/ordered_array.c -o build/ordarray.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/assert.c -o build/assert.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/vfs.c -o build/vfs.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/initrd.c -o build/initrd.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -c -m32 src/impl/task.c -o build/task.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#gcc -c -m32 src/impl/syscall.c -o build/syscall.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

nasm -f elf32 src/impl/gdt.asm -o build/gdtasm.o
nasm -f elf32 src/impl/interrupt.asm -o build/israsm.o
nasm -f elf32 src/impl/process.asm -o build/procasm.o

#build/syscall.o
ld -T linker.ld -melf_i386 -o build/NembusOS.bin build/kernel.o build/task.o build/initrd.o build/vfs.o build/procasm.o build/assert.o build/ordarray.o build/kheap.o build/paging.o build/isr.o build/timer.o build/israsm.o build/gdt.o build/gdtasm.o build/keyboard.o build/cmos.o build/boot.o build/string.o build/io.o build/memory.o build/vga.o build/std.o -build-id=none --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_

if grub-file --is-x86-multiboot build/NembusOS.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

mkdir -p isodir/boot/grub
sudo cp grub.cfg isodir/boot/grub/grub.cfg
sudo cp build/NembusOS.bin isodir/NembusOS.bin

gcc ramdisk.c -o ramdisk.out
sudo ./ramdisk.out initrd/a.txt hello.txt

sudo rm -rf build/kernel.o build/task.o build/initrd.o build/vfs.o build/procasm.o build/assert.o build/ordarray.o build/kheap.o build/paging.o build/isr.o build/timer.o build/israsm.o build/gdt.o build/gdtasm.o build/keyboard.o build/cmos.o build/boot.o build/string.o build/io.o build/memory.o build/vga.o build/std.o ramdisk.out

sudo grub-mkrescue -o build/NembusOS.iso isodir

sudo rm -rf build/NembusOS.bin isodir/NembusOS.bin isodir/initrd

qemu-system-i386 -cdrom build/NembusOS.iso -m 512 -cpu max -no-reboot -d int