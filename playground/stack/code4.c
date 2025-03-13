#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void zot(unsigned long *stop) {
    unsigned long r = 0x3;
    unsigned long *i;
    for (i = &r; i <= stop; i++) {
        printf("%p      0x%lx\n", i, *i);
    }
}

void foo(unsigned long *stop) {
    unsigned long q = 0x2;
    zot(stop);
}

int main() {
    int pid = getpid();
    unsigned long p = 0x1;

    foo(&p);

    back:
    printf("    p: %p  \n", &p);
    printf("    back: %p  \n", &&back);

    printf("\n\n /proc/%d/maps \n\n", pid);

    char command[50];

    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
}

// We should be able to identify the return address after the call to foo() and zot()

// You also see the local data structures: p, q, r and a copy of the address
// to p. If this was a vanilla stack as explained in any school books, you would
// also be able to see the argument to the procedures. However, GCC on a x86
// architecture will place the first six arguments in registers so those will not
// be on the stack. You can add ten dummy arguments to the procedures and
// you will see them on the stack.

// If you do some experiments and encounter elements that can not be
// explained it might be that the compiler just skips some bytes in order to
// keep the stack frames aligned to a 16-byte boundary. The base stack pointer
// will thus always end with a 0.