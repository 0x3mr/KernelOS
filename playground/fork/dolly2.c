#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        printf("I am the child %d\n", getpid());
    } else {
        printf("My child is called %d\n", pid);
    }

    printf("pid = %d\n", pid);

    return 0;
}