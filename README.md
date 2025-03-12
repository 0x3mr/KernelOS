# KernelOS

A simple operating system.

## Authors

- Amr Abdelfattah
- Momen Mahmoud

## Reference

- https://people.kth.se/~johanmon/ose/assignments/boot.pdf

## Project Structure

```
KernelOS/
│── docs/
│── src/
│   ├── playground/
│   │   ├── code.c
│   ├── cdrom/
│   │   ├── boot/
│   │   │   ├── grub/
│   │   │   │   ├── grub.cfg
│   │   │   ├── kernel.bin
│   │   │   ├── boot.bin
│   ├── boot.asm
│   ├── kernel.asm
│   ├── kernel.o
│   ├── linker.ld
│   ├── multiboot_header.asm
│   ├── multiboot_header.o
│── README.md
```