//this code transforms an existing proccess to run code of a given program
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        execlp("ls", "ls", NULL);
        printf("This will only happen if exec fails\n");
    } else {
        wait(NULL);
        printf("We're done\n");
    }

    return 0;
}

/*The call to execlp() will and the program 'ls' and then replace the
  code and data segments of the process with the code and data found in the
  executable binary. The stack and heap areas are reset so the program starts
  the execution from scratch.   */