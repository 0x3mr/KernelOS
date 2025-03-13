#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        int child = getpid();
        printf("I'm the child %d in group %d\n", child, getpgid(child));
    } else {
        int parent = getpid();
        printf("I'm the parent %d in group %d\n", parent, getpgid(parent));
        wait(NULL);
    }

    return 0;
}
/*  this uses "getpgid" to locate the group identifier/leader
    some group properties:
    A group is treated as a unit by the shell, it can set a whole group
    to suspend, resume or run in the background                      */

    