#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <sys/wait.h> // wait() icin

int main(int argc, char** argv){

    if(argc<2)
    {
        fprintf(stderr,"Lutfen bir sayi giriniz\n");
        exit(1);
    }

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork()'larken bir hata meydana geldi");
        exit(1);
    }
    else if(pid == 0){ // CHILD
        int sayi = atoi(argv[1]);
        while(sayi!=1){
            printf("%d ",sayi);
            if(sayi%2==0)
            {
                sayi = sayi/2;
            }
            else{
                sayi=sayi*3+1;
            }
        }
        printf("%d\n",sayi);
    }
    else // PARENT
    {
        int status;
        pid_t ended_process = wait(&status);
    }

    return 0;
}