# List of compiling commands

## `boot.asm`

```
nasm -f bin boot.asm -o boot.bin
```

```
qemu-system-x86_64 -drive file=boot.bin,index=0,media=disk,format=raw
```

## `multiboot_header.asm`

```
nasm -f elf32 multiboot_header.asm
```

## `kernel.asm`

```
nasm -f elf32 kernel.asm
```

## What's inside the object files?

```
readelf -a kernel.o
```
"The program will read an ELF object ﬁle
and display tons of information that’s mostly mostly Greek (unless you’re
from Greece, then it is mostly Chinese)."

## Linking the object files

```
ld -m elf_i386 -o kernel.bin -T linker.ld multiboot_header.o kernel.o
```

# Creating the ISO image using `grub-mkrescue`

```
grub-mkrescue -o cdrom.iso cdrom
```

Inspect the imagine file:
`file cdrom.iso`
`isoinfo -d -i cdrom.iso` (yay -S cdrtools)

# Let's run it.. shall we?

```
qemu-system-x86_64 -cdrom cdrom.iso
```

# How can I see the output?

```
vncviewer 127.0.0.1:5900
```