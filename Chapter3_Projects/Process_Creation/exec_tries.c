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
        // exec family'de ilk arguman yine komutun path'i olmali.
        // 3. parametre olarak asil calsitirmak istedigimiz komutlara
        // parametre yani arguamn gecmeye basliyoruz.
        // Egerki daha fazla bir parametre gecmeyeceksek NULL parametresi
        // gecerek daha fazla parametre gecmeyecegimizi belirtiyoruz.

        // execl ile execlp arasındaki fark ise sondaki p ifadesinden ayirt edilebilecegi gibi
        // execlp PATH variable icerisine bakiyor ve senin tam bir dosya yolu vermene gerek kalmiyor
        // Ancak execl'de ise tam bir yol vermen gerek

        // execl ile execv ise birisi parametreleri pointer seklinde digeri ise 
        // vector(yani liste) seklinde tutuyor. 
        execlp("ls","ls","-l",NULL);
    }
    else // PARENT
    {
        int status;
        pid_t ended_process = wait(&status);
        printf("Tamamlandi\n");
    }
    return 0;
}