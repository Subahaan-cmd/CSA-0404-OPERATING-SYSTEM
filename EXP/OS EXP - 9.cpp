#include <stdio.h>
#include <windows.h>
#include <string.h>

#define SHARED_NAME "MySharedMemory"

int main(int argc, char *argv[])
{
    HANDLE hMapFile;
    char *sharedMemory;

    /* Child process */
    if (argc > 1 && strcmp(argv[1], "child") == 0)
    {
        hMapFile = OpenFileMappingA(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            SHARED_NAME
        );

        if (hMapFile == NULL)
        {
            printf("Unable to open shared memory.\n");
            return 1;
        }

        sharedMemory = (char *)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            1024
        );

        if (sharedMemory == NULL)
        {
            printf("Unable to access shared memory.\n");
            CloseHandle(hMapFile);
            return 1;
        }

        printf("\nChild Process\n");
        printf("Message received: %s\n", sharedMemory);

        UnmapViewOfFile(sharedMemory);
        CloseHandle(hMapFile);

        return 0;
    }

    /* Parent process */
    hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        1024,
        SHARED_NAME
    );

    if (hMapFile == NULL)
    {
        printf("Unable to create shared memory.\n");
        return 1;
    }

    sharedMemory = (char *)MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        1024
    );

    if (sharedMemory == NULL)
    {
        printf("Unable to access shared memory.\n");
        CloseHandle(hMapFile);
        return 1;
    }

    /* Write message into shared memory */
    strcpy(sharedMemory, "Hello from Parent Process!");

    printf("Parent Process\n");
    printf("Message written to shared memory.\n");

    /* Create child process */
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char commandLine[MAX_PATH + 20];

    sprintf(commandLine, "\"%s\" child", argv[0]);

    if (CreateProcessA(
            NULL,
            commandLine,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        printf("Unable to create child process.\n");
    }

    /* Clean up */
    UnmapViewOfFile(sharedMemory);
    CloseHandle(hMapFile);

    return 0;
}