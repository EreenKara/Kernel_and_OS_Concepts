/**
 * Simple program demonstrating Win32 anonymous pipes.
 *
 * Figures 3.23 & 3.24
 *
 * @author Silberschatz, Gagne and Galvin.
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */


// Windows'ta child process'in, parent process'in hangi ozelliklerini inherit edecegini belirtmemiz gerekir.
// ornegin pipe bunlardan biridir. Ozel olarak belirtmemiz gerekir.
// Bunu ise SECURITY_ATTRIBUTES ozelligi ile yapiyoruz.

 
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
    // bu HANDLE'i file descriptor gibi dusunebilirsin
    // okuma ve yazma ucuna isaret ediyor.
    HANDLE ReadHandle, WriteHandle;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char message[BUFFER_SIZE] = "Greetings";
    DWORD written;

    /* set up security attributes so that pipe handles are inherited */
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL,TRUE};

    /* allocate memory */
    ZeroMemory(&pi, sizeof(pi));

    /* create the pipe */
    /**
     * 1. parametre reading handle
     * 2. parametre writing handle
     * 3. parametre SECURITY_ATTRIBUTES ki biz soyluyoruz hangi kurallar gecerli olacak bu pipe'ta
     * 4. parametre size of pipe in bytes.
     */
    if ( !CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
    fprintf(stderr,"Create Pipe Failed\n");
    return 1;
    }

    /* establish the START_INFO structure for the child process */
    GetStartupInfo(&si);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    /* redirect the standard input to the read end of the pipe */
    si.hStdInput = ReadHandle;
    si.dwFlags = STARTF_USESTDHANDLES;

    /* we do not want the child to inherit the write end of the pipe */
    SetHandleInformation( WriteHandle, HANDLE_FLAG_INHERIT, 0); 

    /* create the child process */
    // 5. parametre'yi true yaptik ki cocuk process parent'in sahip oldugu pipe'i inherit edebilsin.
    if (!CreateProcess(NULL, 
                        ".\\child.exe",
                        NULL,
                        NULL,
                        TRUE, /* inherit handles */
                        0,
                        NULL,
                        NULL,
                        &si,
                        &pi))
    {
        fprintf(stderr, "Process Creation Failed\n");
        return -1;
    }
                
    /* close the unused end of the pipe */
    CloseHandle(ReadHandle);
        
    /* the parent now wants to write to the pipe */
    if (!WriteFile (WriteHandle, message, BUFFER_SIZE, &written, NULL))
    fprintf(stderr, "Error writing to pipe\n");

    /* close the write end of the pipe */
    CloseHandle(WriteHandle);

    /* wait for the child to exit */
    WaitForSingleObject(pi.hProcess, INFINITE);  

    /* close all handles */
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);                 
}
