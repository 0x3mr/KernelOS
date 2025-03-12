# KernelOS

A simple operating system.

## Authors

- Amr Abdelfattah
- Momen Mahmoud

## Reference

- https://people.kth.se/~johanmon/ose/assignments/boot.pdf

- https://people.kth.se/~johanmon/ose/assignments/stack.pdf
- https://people.kth.se/~johanmon/ose/assignments/fork.pdf

## Project Structure

```
KernelOS/
│
├── docs/                       # Documentation for the project
│   ├── executables.md          # Compiling commands used within the project
│   └── packages.md             # Dependencies to install to get the project running
├── playground/                 # Experimental code and testing ground
│   └── code.c                  # Sample C code for testing memory and processes
├── src/                        # Source code directory
│   ├── cdrom/                  # Bootable CD-ROM structure
│   │   └── boot/               # Boot directory
│   │       ├── grub/           # GRUB bootloader configuration
│   │       │   └── grub.cfg    # GRUB boot menu configuration
│   │       ├── kernel.bin      # Compiled kernel binary
│   │       └── boot.bin        # Boot sector binary
│   ├── boot.asm                # Assembly code for bootstrapping
│   ├── kernel.asm              # Kernel assembly code
│   ├── kernel.o                # Compiled kernel object file
│   ├── linker.ld               # Linker script
│   ├── multiboot_header.asm    # Multiboot header for bootloaders
│   └── multiboot_header.o      # Compiled multiboot header object
└── README.md                   # Project documentation and setup guide
```