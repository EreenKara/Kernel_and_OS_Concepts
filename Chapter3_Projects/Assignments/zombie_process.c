/**
 * 10 saniye boyunca duracak olan zombie child process olusturmaya calisiyoruz.
 * Zombie process, sonlanmis olan ancak parent'inin wait() cagirmadigi process'tir. 
 * Child process bitmis olmasina ragmen process table'da hala degerleri tutuluyordur.
 * Orphan ile karistirilmamali. Orphan'da process'in parent'i sonlanmis ve wait cagirmamistir.
 */

#include <stdio.h>
#include <unistd.h> // sleep() bunda bulunuyor.
#include <stdlib.h> // exit() fonksiyonu buna ait

int main(){
    pid_t pid;
    pid = fork();

    if(pid < 0 ){ // ERROR
        perror("Process olusurken hata oldu");
        exit(1);

    }
    else if(pid == 0){// CHILD
        printf("Child proces olustu ve id'si:  %d\n",getpid());
        exit(1);
    }
    else{ // PARENT => pid>0
        printf("Parent process waiting for ten seconds. parent process id'i: %d\n",getpid());
        // sleep calisirken shell command algilamiyor. o yuzden asagidaki komutu calistirmasi icin kullandim.
        system("ps -l");
        sleep(3); // 3 saniye boyunca uyutacagim.
        //char buff[32];
        //sprintf(buff, "kill -9 %d", getpid());
        //system(buff);
        system("ps -l");
        sleep(9); // 9 saniye boyunca uyutacagim.
        printf("Beklemesi bitti.\n");

    }

    return 0;
}