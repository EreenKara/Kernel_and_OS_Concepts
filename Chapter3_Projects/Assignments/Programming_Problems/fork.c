#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <sys/wait.h> // wait() icin

int main(int argc,char** argv)
{
    pid_t pid;
    pid = fork();
    if(pid<0){ // ERROR
        perror("FORK'TA HATA");
        exit(1);
    }   
    else if(pid == 0 ){ // CHILD

    }
    else // PARENT
    {
        int status;
        pid_t ended_process = wait(&status);

    }
    return 0;
}