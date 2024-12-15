#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <sys/wait.h> // wait() icin

#include <fcntl.h> // open() flaglarini iceriyor
// #include <sys/shm.h>
#include <sys/stat.h> // mode constants 
#include <sys/mman.h> // asil shm_open flena burda bulunuyor.

#define SIZE 2048
#define NAME "collatz"
int main(int argc, char** argv){

    if(argc<2)
    {
        fprintf(stderr,"Lutfen bir sayi giriniz\n");
        exit(1);
    }
    if(argv[1]<=0)
    {
        fprintf(stderr,"Lutfen bir pozitif bir sayi giriniz\n");
        exit(1);
    }
    
    int shm_memory_obj = shm_open("collatz", O_CREAT | O_RDWR, 0666);
    if(shm_memory_obj<0) 
    {
        perror("HATA");
        exit(1);
    }
    ftruncate(shm_memory_obj,SIZE);
	void* ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_memory_obj, 0);

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork()'larken bir hata meydana geldi");
        exit(1);
    }
    else if(pid == 0){ // CHILD
        int sayi = atoi(argv[1]);
        while(sayi!=1){
            *((int*)ptr)=sayi;
            ptr+=sizeof(int);
            if(sayi%2==0)
            {
                sayi = sayi/2;
            }
            else{
                sayi=sayi*3+1;
            }
        }
        *((int*)ptr)=sayi;
        exit(0);
    }
    else // PARENT
    {
        int status;
        pid_t ended_process = wait(&status);
        int sayi =0;
        while(sayi!=1)
        {
            sayi =*(int*)ptr;
            ptr+=sizeof(int);
            printf("%d ",sayi);
        }
        /* remove the shared memory segment */
        // file'i silme islemi. olusturdugumuz name'i belirterek siliyoruz.
        if (shm_unlink(NAME) == -1) {
            printf("Error removing %s\n",NAME);
            exit(-1);
	    }
    }

    return 0;
}