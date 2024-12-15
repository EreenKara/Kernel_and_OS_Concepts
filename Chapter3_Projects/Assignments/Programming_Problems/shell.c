#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit() fonksiyonu buna ait
#include <sys/wait.h> // wait() icin
#include <string.h>
#include <fcntl.h> // open() sistem çağırsı için flag'ların tanımlandıgı header


typedef enum {FALSE,TRUE} bool;

#define MAX_LINE 80
#define BUFF_SIZE 1024

#define READ_END	0
#define WRITE_END	1

int split_data(char*,char**);
bool check_character(char**,int, char*);
char* screen();
char** start_arguments();
int index_of_command(char** buff,int size, char* istenilen_string);

int main(int argc,char** argv)
{
    bool should_run = TRUE; // flag to determine when to exit program
    bool should_wait = FALSE;
    char * last_commands= NULL;
    while (should_run)
    {   
        char* text_from_screen = screen(); // free after use
        // Enter tusuna direkt basilmasi durumu, NULL hatasi yemeyelim diye.
        while(strcmp(text_from_screen,"")==0&&strcmp(text_from_screen,"\0")==0)
        {
            free(text_from_screen);
            text_from_screen = screen();
        }
        
        char* modified_text=strdup(text_from_screen); // free after use
        char** args=start_arguments(); // free after use
        int token_number = split_data(modified_text,args);
        /**
         * Egerki "!!" varsa son calistirilan komutlari kullan.
         */
        if(strcmp(args[0],"!!")==0 && last_commands==NULL){
            printf("KAYITTA HIC KOMUT YOK.\n");
            free(text_from_screen);
            free(modified_text);
            free(args);
            continue;   
        }
        else if(strcmp(args[0],"!!")==0)
        {
            free(text_from_screen);
            free(modified_text);
            text_from_screen=last_commands;
            char* modified_text=strdup(text_from_screen);
            token_number= split_data(modified_text,args);
        }
        
        



        should_wait = check_character(args,token_number,"&") ? FALSE :TRUE;



        






        pid_t pid = fork();
         
        if(pid<0){ // ERROR
            perror("FORK'TA HATA");
            exit(1);
        }   
        else if(pid == 0 ){ // CHILD
            
            if(!should_wait) // & komutu bizim icin anlamli, execvp'ye gitmemesi gerek.
            {
                args[index_of_command(args,token_number,"&")]=NULL;
                token_number--;
            }

            
            if(TRUE == check_character(args,token_number,"<"))
            {
                int command_index = index_of_command(args,token_number,"<");
                int fd = open(args[command_index + 1], O_RDONLY);
                if(fd< 0) {perror("Dosya read_only olarak acilmaya calisirken karsilasilan hata"); exit(1);}
                dup2(fd,STDIN_FILENO);
                args[command_index]=NULL;
                args[command_index+1]=NULL;
            } 
            else if(TRUE == check_character(args,token_number,">")){
                int command_index = index_of_command(args,token_number,">");
                int fd = open(args[command_index + 1], (O_WRONLY|O_CREAT|O_TRUNC),0644);
                if(fd< 0) {perror("Dosya write_only olarak acilmaya calisirken karsilasilan hata"); exit(1);}
                dup2(fd,STDOUT_FILENO);
                args[command_index]=NULL;
                args[command_index+1]=NULL;

            }// 0 1 2 3 4 5 6 7  -> 8
            else if(TRUE == check_character(args,token_number,"|")){
                printf("| karakteri var \n");
                int command_index = index_of_command(args,token_number,"|");
                int size_child_arguments = command_index;
                int size_child_child_arguments = token_number - command_index - 1;
                if(token_number <= size_child_arguments+1) {printf("| karakterinden sonra bir komut girmediniz.\n");exit(1);}
                char** child_arguments = (char**)malloc(sizeof(char*) * size_child_arguments+1);
                char** child_child_arguments = (char**)malloc(sizeof(char*) * (size_child_child_arguments+1));
                int fd[2];
                if (pipe(fd) == -1) {
                    fprintf(stderr,"Pipe failed");
                    return 1;
                }
                pid_t child_of_child_process = fork();
                if(child_of_child_process<0) // ERROR
                {
                    perror("HATA FORK COCUK PROCESS");
                    exit(1);
                }
                else if(child_of_child_process==0) // CHILD
                {
                    close(fd[WRITE_END]);
                    for (int i = 0; i < size_child_child_arguments; i++)
                    {
                        child_child_arguments[i] = args[size_child_arguments+1+i];
                    }
                    child_child_arguments[size_child_child_arguments]=NULL;
                    
                    dup2(fd[READ_END],STDIN_FILENO);
                }
                else{ // PARENT
                    close(fd[READ_END]);
                    for (int i = 0; i < size_child_arguments; i++)
                    {
                        child_arguments[i] = args[i];
                    }
                    child_arguments[size_child_arguments] = NULL;

                    dup2(fd[WRITE_END],STDOUT_FILENO);
                }
            }
            
            // exec family'de ilk ve ikinci arguman yine komutun path'i olmali.
            // 3. parametre olarak asil calsitirmak istedigimiz komutlara
            // parametre yani arguman gecmeye basliyoruz.
            // Egerki daha fazla bir parametre gecmeyeceksek NULL parametresi
            // gecerek daha fazla parametre gecmeyecegimizi belirtiyoruz.

            // execl ile execlp arasındaki fark ise sondaki p ifadesinden ayirt edilebilecegi gibi
            // execlp PATH variable icerisine bakiyor ve senin tam bir dosya yolu vermene gerek kalmiyor
            // Ancak execl'de ise tam bir yol vermen gerek

            // execl ile execv ise birisi parametreleri pointer seklinde digeri ise 
            // vector(yani liste) seklinde tutuyor. 
            execvp(args[0],args);
            // exit satirina ulasmayacak ancak herhangi bir hata olursa diye.
            exit(1);
        }
        else // PARENT
        {
             
            /**
             * Burada loop halinde bekletmemin sebebi "&" isareti oldugunda beklemedigimiz
             * process illaki sonlaniyor biz uzerinden gectikten sonra. Bir sonraki komut giriliyor ve onda & isareti yok
             * o zaman beklemeliyiz ancak bu seferde daha onceki beklemedigimiz process wait()'e donuyor ve wait()'in asil
             * beklemesi gereken process'i beklememesine sebep oluyor. ZOMBIE process muhabbeti yani.
             * Bu yuzden wait loop'una soktum.
             */
            if(should_wait){ // Process bekleyecek
                int status;
                // printf("Komutun sonlanmasi bekleniyor.\n");
                pid_t ended_process = wait(&status);
                while (ended_process != pid)
                {
                    ended_process = wait(&status); 
                }
                
                // printf("Komutun SONLANDI.\n");
            }

            free(args);
            free(last_commands);
            last_commands = strdup(text_from_screen);
            free(text_from_screen);
            free(modified_text);
        }
        // printf("\n");
    }
    
    printf("Program Sonlandi\n");
    return 0;
}

/**
 * you have to take care of return value. free after use it.
 */
char* screen()
{
    char * buff = (char*)malloc(sizeof(char)*BUFF_SIZE);
    // printf normalde ekrana cikartiyor ancak 
    // cok hizli yazarsak internal buffer'ina yaziyor bizim ekrana cikartmak istediklerimizi
    // biriktiriyor biriktiriyor ve ondan sonra ekrana basiyor. Bunun onune gecmek icin fflush kullaniliyor.
    printf("osh>");
    fflush(stdout);
    // fgets sondaki \n karakterini de okuyor bunu atmak gerek string'den ahtaya sebep oluyor.
    fgets(buff, BUFF_SIZE, stdin);
    size_t len = strlen(buff);
    if (len > 0 && buff[len - 1] == '\n') {
        buff[len - 1] = '\0';
    }
    return buff;
}

char** start_arguments(){
    char** args =(char**)malloc(sizeof(char*)*(MAX_LINE/2+1)) ; // command line arguments
    for (int i = 0; i < MAX_LINE / 2 + 1; i++) {
        args[i] = NULL;
    }   
    return args;
}

/**
 * Kac tane data'ya ayrildi onu dondurur. argc yani.
 * splitted_data'nin icerigini token'lar ile doldurur.
 * NULL'uda token'lara dahil eder.Son eleman olarak.
 * 
 * splitted data buffer'daki kisimlara isaret ediyor
 * buffer free()'lernirse splitted data'da isaretlerde sikintiya girer.
 */
int split_data(char* buff,char** splitted_data)
{
    char delim[] = " ";  // Delimiters are space, comma, period, and exclamation mark
    char *token;
    // char internal_buffer[BUFF_SIZE];
    // strcpy(internal_buffer,buff);

    // strtok() bir pointer donduruyor. Bu pointer kendisine ayrilmak uzere 
    // verilen string'in icerisinde bir noktaya point ediyor.
    // yani ben burada egerki stack'te olusturulmus veriyi strtok()'a verirsem o da oraya isaret edicek.
    // fonksiyon sonlaninca ayirdigim butun token'lar bosa gidicek. Cunku stack'e isaret ediyordu.
    token= strtok(buff, delim);
    // ayrica strtok kendisine verilen string uzerinde degistirme yaptigindan 
    // "" (string literal) veya constant string'ler ile islem yapilamiyor. 
    splitted_data[0] =token;

    int i=1;
    /**
     * PARCALARA AYIRIR.
     * Son NULL'u da ekleyerek splitted data'yi tamamlar. 
     */
    for(;token!=NULL && i<(MAX_LINE/2+1);i++) 
    {
        // ayni stringi kullanmak icin NULL veriyormusuz.
        token = strtok(NULL, delim);
        splitted_data[i]=token;
    }
    
    return i-1;
}

/**
 * char dizisi icerisinde istenilen bir karakteri buluyor
 */
bool check_character(char** buff,int size, char* istenilen_string)
{
    for (size_t i = 0; i < size; i++)
    {
        if(strcmp(buff[i],istenilen_string)==0) return TRUE;     
    }
    return FALSE;
}

int index_of_command(char** buff,int size, char* istenilen_string)
{
    for (size_t i = 0; i < size; i++)
    {
        if(strcmp(buff[i],istenilen_string)==0) return i;     
    }
    return -1;
}


