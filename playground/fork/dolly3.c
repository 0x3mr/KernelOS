//this is the "extension of dolly" section 2.4 "clone of a process"

#include <stdio.h>
#include <unistd.h>

int main(){
    int pid;
    int x = 123;
    pid = fork();

    if(pid == 0){
        printf("child:\nx is: %d\nand the address is: 0x%p\n",x , &x);
        x = 42;     // update 'x' to showcase that each process has its own variables (no sharing)(it's just copying)
        sleep(1);   //wait 1 sec for human to process what is going on
    }
    else{
        printf("mother:\nx is: %d\n",x);
        x = 13;
        sleep(1);
        printf("mother:\nx is:  %d\n",x);
        wait(NULL);
    }
    return 0;
}
/*this is the same as our quiz 2 (more or less) */