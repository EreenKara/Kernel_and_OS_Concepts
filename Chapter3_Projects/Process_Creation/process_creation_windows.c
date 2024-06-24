/**
 * This program creates a separate process using the CreateProcess() system call.
 *
 * Figure 3.10
 *
 * @author Gagne, Galvin, Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */
 
#include <windows.h>
#include <stdio.h>

int main( VOID )
{
    // Process'in baslangic bilgilerini kaydediyor. windows size, apperance, handles to standart input, output files.
    STARTUPINFO si;
    // Process information HANDLE tutuyor yani bu pi'i kullanarak bizler yeni olusturdugumuz process'e ulasabilecegiz.
    // HANDLE'in genel olarak anlami isaretci gibi dusunebilir. Pointer degil sadece bu handle ilgili bir seyi isaret ediyor.
    // HANDLE kullanilarak artik neyin Handle'i ise o seyi isaret edebiliyoruz.
    PROCESS_INFORMATION pi;

    // memory alocation
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process.
    // 1. parametre application name iken
    // 2. parametre command line parametre oluyor
    // 1. parametre null ise 2. parametre hangi programin yuklenecegini belirtmis oluyor.
    if( !CreateProcess( NULL,   // No module name (use command line).
        "C:\\WINDOWS\\system32\\mspaint.exe", // Command line.
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        FALSE,            // Set handle inheritance to FALSE.
        0,                // No creation flags.
        NULL,             // Use parent's environment block.
        NULL,             // Use parent's starting directory.
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    )
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return -1;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

