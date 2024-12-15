/**
 * GElen mesaji counter case yapip geri gonderme.
 * Bir pipe'i bir process'te okuyup yazabiliyoruz.
 * Diger process'te de okuyup yazabiliyoruz. Ama pipe icerisinde
 * bir queue tuttugundan mesajlarin sirasi ve durumu garanti edilemiyor bu yuzden
 * iki adet pipe kullanmak mantikli.
 * 
 * Pipe kullanirken egerki herhangi bir process tarafinda pipe'in bir tarafi acik degilse islem yapilamiyor.
 * Ornegin ben Write ucundan yazdim ancak Read ucununun acik oldugu herahngi bir process yoksa, write() fonksiyonunda
 * hata veriyor program o satirdan sonrasi duzgun calismiyor program garip bir terminate ediliyor.
 * Bu yuzden pipe'in bir diger ucu acik oldugu surece islem yapilabiliyor.
 */


#include <stdio.h>
#include <unistd.h> // fork() ve pid_t bu header'da
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <sys/wait.h> // wait() icin
#include <string.h> // 

#include <ctype.h> // tolower() ve toupper()

#define READ_END 0 
#define WRITE_END 1


void counter_case(char* ,int );

int main(){
    const int buff_size=100;
    char buff[buff_size];
    int fd_parent[2];
    int fd_child[2];
    /* create the pipe */
	if (pipe(fd_parent) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
    if (pipe(fd_child) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
    pid_t pid;
    pid = fork();
    if(pid < 0){ // ERROR 
        perror("Fork'larken bir hata oldu");
        exit(1);
    }
    else if(pid == 0){ // CHILD
        strcpy(buff,"Naber");
        printf("Cocuk process buff: %s\n",buff);
        close(fd_child[WRITE_END]);
        close(fd_parent[READ_END]);
        write(fd_parent[WRITE_END],buff,strlen(buff));
        close(fd_parent[WRITE_END]);
        close(fd_child[READ_END]);

        
    }
    else // PARENT
    {
        
        int status;
        pid_t which_process_ended = wait(&status);
        // Kullanamyacagim uclari kapatiyorum. Kullanmakta sikinti yok
        // Sadece her bir pipe bir queue'ye sahip oldugundan ayni uca yazdigimizda
        // hangi veri kime ait karisiyor. Bu yuzden 2 pipe kullanmak gerekli.

        // asagidaki ucu kapattigimda program hata veriyor.
        // close(fd_child[READ_END]);
        close(fd_parent[WRITE_END]);
        int okunan = read(fd_parent[READ_END],buff,buff_size);
        printf("Buff counter-case oncesi: %s\n",buff);
        counter_case(buff,okunan);
        write(fd_child[WRITE_END],buff,buff_size);
        // read(fd_child[READ_END],buff,buff_size);
        close(fd_parent[READ_END]);
        close(fd_child[WRITE_END]);

        printf("Buff counter-case sonrasi: %s\n",buff);
    }
    return 0;
}


void counter_case(char* buff,int size){

    for (size_t i = 0; i < size; i++)
    {
        if(buff[i] >= 'A' && buff[i] <= 'Z')
        {
            buff[i]=tolower(buff[i]);
        }
        else if(buff[i] >= 'a' && buff[i] <= 'z')
        {
            buff[i]=toupper(buff[i]);
        }
    }
}