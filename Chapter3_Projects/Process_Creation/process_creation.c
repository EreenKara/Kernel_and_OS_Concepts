#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid; // integer bir sayi ile de tutulabilir. ancak process_id_type olarak bu kullaniliyor.

    // Cocuga 0 donerken parent'e cocugun procees id'si doner
    pid= fork(); // Yeni bir prcoess yaratmak için.
    if(pid<0)
    {
        // Normalde fprintf kullanilmis ancak bence burda perror kullanmak daha mantikli
        // fprintf(stderr, "Fork failed");
        // Sistem tarafindan uretilen bir hata'da hatayi belirlemek icin perror kullanmak daha mantikli
        perror("Process olusturulurken hata");
        // exit(1);
        // Return kullanilmis exit(1)'de kullanilabilirdi. Ancak dikkat et 0 degil
        // 0 problemsiz bir bitisi temsil ediyor.
        return 1;
    }
    else if(pid==0) // CHILD
    {
        // Burada process'in ramdeki alanını yepyenibir uygulama ile yukelyerek process'in baska bir gorevde calismasini sagliyoruz.
        execlp("/bin/ls","ls",NULL);
    }
    else{ // PARENT
        // child process'ten bitirdigine dair haber alana kadar burada bekleyecek. Ondan sonra devam edecek. 
        pid_t pid_sonlanan;
        int status;
        // wait donus degeri oalrak hangi process'in terminate edildigini tutuyor
        // icerisine gectigimiz parametreye ise biten process'in biterken exit olarak hangi sayi ile bittigini belirtiyor.
        pid_sonlanan=wait(&status);
        printf("Child Complete");

    }

    return 0;
}