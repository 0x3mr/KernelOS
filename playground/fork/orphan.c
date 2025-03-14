#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
        int child = getpid();
        printf("child: parent %d, group %d\n", getppid(), getpgid(child));
        sleep(4);
        printf("child: parent %d, group %d\n", getppid(), getpgid(child));
        sleep(4);
        printf("child: parent %d, group %d\n", getppid(), getpgid(child));
    } else {
        int parent = getpid();
        printf("parent: parent %d, group %d\n", getppid(), getpgid(parent));
        sleep(2);

        // Prevent division by zero error
        int zero = 0;
        if (zero != 0) {
            int i = 3 / zero;  // This line would cause a runtime error
        } else {
            printf("Attempted division by zero prevented.\n");
        }
    }

    return 0;
}
