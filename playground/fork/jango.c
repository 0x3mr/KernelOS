#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid == 0) {
              /*these are flags (statements on the status to open this file with)
                O_RDWR   Open file for both reading and writing.
                O_CREAT  Create file if it does not exist.
                S_IRUSR  Grant read permission to the file owner. 
                S_IWUSR  Grant write permission to the file owner.*/
        int fd = open("quotes.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); 
        if (fd == -1) {
            perror("open");
            return 1;
        }

        dup2(fd, 1);
        close(fd);

        execlp("boba", "boba", NULL);
        perror("execlp"); // Print error if exec fails
        printf("This only happens on failure\n");
    } else {
        wait(NULL);
    }

    return 0;
}
/* inside the file descriptor table, the first entry will be our table entry (fd)
   then we will use dup2 (line 23) to copy the entry to position (1) -> is the location of standard output
   then we will close the (fd) entry as we will not use it anymore since we already cloned it
   now we will call execlp. this will turn the process into 'boba'
   the 'boba' program knows nothing of what is going on.
   it will redirect its output to the first entry of the file descriptor table as per usual
   but we changed the first entry to be our program that redirects that output to the file 'quotes.txt' instead of shell*/