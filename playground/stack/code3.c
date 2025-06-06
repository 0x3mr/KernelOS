#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int pid = getpid();
    unsigned long p = 0x1;

    printf("p (0x%lx) : %p \n", p, &p);
    printf("\n\n /proc/%d/maps \n\n", pid);

    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
}

// Location of p in the stack segment
// Use `setarch $(uname -m) -R bash`
// to stop the stack segment from moving bits around