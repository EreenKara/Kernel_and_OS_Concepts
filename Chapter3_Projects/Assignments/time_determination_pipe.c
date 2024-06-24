/**
 * Herhangi bir komutun calisma suresini bulan program.
 * Bunu child process bir komutu calistirip sonlaninca onun calistirma suresini hesaplayarak buluyoruz.
 * IPC ve timeval kullaniyoruz. 
 * gettimeofday() 1 ocak 1970'den bu yana gecen saniyeyi donduruyor.
 * timeval ise icerisinde tv_sec ( seconds ) ve tv_usec ( microseconds ) barindiran iki field 
 * mikrosaniye olan her bir saniye arasindakini gosteriyor. 1970'den beri kaç mikrosaniye geçti değil.
 * her bir saniye artimi icin gecmesi gereken mikrosaniyeyi tutuyor.0'dan basiyor 100 olunca 1 saniye olmus gibi dusun.
 * Ben sadece kac saniye suruyor bir program onu buldum. mikro saniyeyle ugrasmadim tekrar.
 */

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// KUTUPHANE OLARAK rt kutuphanesini eklemeliyiz derlenmesi icin
// gcc test.c -o program -lrt
// seklinde bir derleme yaparak kullanabiliyorum shared memory'yi.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // _t ile biten bircok type bunun icerisnde yer aliyor.
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <fcntl.h> // O_CREAT | O_RDWR icin

// SHARED MEMORY icin hangisi ne yapiyor tam olarak bilmiyorum.
#include <sys/shm.h>
#include <sys/stat.h> // mode constants 
#include <sys/mman.h> // shm_open flena burda bulunuyor.

#include <sys/time.h> // gettimeofday, timeval burada bulunuyor 
#include <sys/wait.h> // wait() icin


#define SIZE 4096
#define READ_END	0
#define WRITE_END	1


int main(int argc,char** argv)
{
    
    if(argc<2) // kullanici istenilen parametreyi girmemisse
    {
        fprintf(stderr,"Lutfen calistiralacak olan komutu parametre olarak giriniz.\n");
        exit(1);
    }

    // shared memory'yi kurmak
	
	int fd[2];

	/* create the pipe */
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

    pid_t pid;
    pid = fork();

    if(pid < 0){ // ERROR
        perror("Fork'larken bir ahta oldu");
        exit(1);
    }
    else if(pid == 0) // CHILD 
    {
        struct timeval start_time_timeval;
        gettimeofday(&start_time_timeval,NULL);

        // void pointer'a bir yazma islemi yaptigimizdan 
        // ve yer ayirma islemini zaten mmap ile ftruncate ile yukarida 5096 bye olacak sekilde yaptigimizdan
        // Normalde nasil bir pointer'a deger atiyorsak, void *'ında ilgili adresine ayni sekikde deger atiyoruz.
        printf("zaman: %lu\n",start_time_timeval.tv_sec);
        
        close(fd[READ_END]);

		/* write to the pipe */
        const int buff_size=100; 
        char buff[buff_size];
        // bir sayisal deger yazmak isitoyur zama pipe sadece string degerler yazilabilen bir sey
        // bu yuzden ilk once istedigimiz degeri strigne ceviriyoruz.
        sprintf(buff,"%lu",start_time_timeval.tv_sec);
        //write'a direkt olarak bir integer deger veremiyoruz.
		write(fd[WRITE_END], buff, buff_size); 

		/* close the write end of the pipe */
		close(fd[WRITE_END]);
        // ASAGIDAKI SEKILDE YAPINCA OLMUYOR
        // GECMEK ISTEDGIMIZ PARAMETRE BIR STRING DEGIL O YUZDEN ASAGIDAKI SEKILDE GECEMEZSIN
        // POINTER MANTIGI ILE DUSUNEREK YAPMAK GEREK.
        // sprintf(ptr,"%lu",(long)222222);
        // ptr+=sizeof(long);


        if(execlp(argv[1],argv[1],NULL)<0)
        {
            // Hata var diye shared memory'ye yaz.
            perror("Execlp() hatasi");
            exit(1);
        }
        
    }
    else // PARENT
    {
        int status;
        pid_t which_process_ended;
        which_process_ended = wait(&status);
        close(fd[WRITE_END]);

        const int buff_size=100;
        char buff[buff_size];
        int readed = read(fd[READ_END],buff,buff_size);
        long start_time = atoi(buff);
        close(fd[READ_END]);
        
        //printf("sizeof: %lu",*((long*)ptr));
        
        // ptr ile bir sonrakini okumak istiyorsan ptr'yi o kadar arttirman gerek ornegin bu ornekte 
        // long kadar bir arttirim yapmak gerek. Cunku biz shared memory'ye long yazdigimizi biliyoruz.

	    // ptr += sizeof(long);
        // printf("sayi 2 : %lu",*((long*)ptr));

        struct timeval end_time_timeval;
        gettimeofday(&end_time_timeval,NULL);

        long end_time=end_time_timeval.tv_sec;
        printf("start sure: %lu\n",start_time);
        printf("end sure: %lu\n",end_time);

        long time_interval = start_time-end_time;
        printf("Gecen sure: %lu\n",time_interval);

    }

    return 0;
}


// bu sekilde hesaplamak daha kolay diyorlardi internette.
int time_h_fonk(){
    /* #include <time.h>  */
    // clock_t start = clock();
    // //... do work here
    // clock_t end = clock();
    // double time_elapsed_in_seconds = (end - start)/(double)CLOCKS_PER_SEC;
}