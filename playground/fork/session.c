#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        int child = getpid();
        printf("child: session %d\n", getsid(child));
    } else {
        int parent = getpid();
        printf("parent: session %d\n", getsid(parent));
    }

    return 0;
}


/* A session consists of a set of groups and a session leader. As with
   groups, the sessions have identifiers that are equal to the leaders process
   identifier.          */