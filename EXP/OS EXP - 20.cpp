
#include <windows.h>
#include <stdio.h>

HANDLE resource;
HANDLE readMutex;

int readers = 0;
int sharedData = 0;

DWORD WINAPI reader(LPVOID arg)
{
    int id = *(int *)arg;
    int value;

    WaitForSingleObject(readMutex, INFINITE);
    readers++;

    if (readers == 1)
        WaitForSingleObject(resource, INFINITE);

    ReleaseMutex(readMutex);

    value = sharedData;
    printf("Reader %d reads value: %d\n", id, value);
    Sleep(200);

    WaitForSingleObject(readMutex, INFINITE);
    readers--;

    if (readers == 0)
        ReleaseSemaphore(resource, 1, NULL);

    ReleaseMutex(readMutex);

    return 0;
}

DWORD WINAPI writer(LPVOID arg)
{
    int id = *(int *)arg;

    WaitForSingleObject(resource, INFINITE);

    sharedData++;
    printf("Writer %d writes value: %d\n", id, sharedData);
    Sleep(200);

    ReleaseSemaphore(resource, 1, NULL);

    return 0;
}

int main(void)
{
    HANDLE threads[4];
    int ids[4] = {1, 2, 1, 2};
    int i;

    resource = CreateSemaphore(NULL, 1, 1, NULL);
    readMutex = CreateMutex(NULL, FALSE, NULL);

    if (resource == NULL || readMutex == NULL) {
        printf("Synchronization object creation failed.\n");
        return 1;
    }

    threads[0] = CreateThread(NULL, 0, reader, &ids[0], 0, NULL);
    threads[1] = CreateThread(NULL, 0, reader, &ids[1], 0, NULL);
    threads[2] = CreateThread(NULL, 0, writer, &ids[2], 0, NULL);
    threads[3] = CreateThread(NULL, 0, writer, &ids[3], 0, NULL);

    for (i = 0; i < 4; i++) {
        if (threads[i] == NULL) {
            printf("Thread creation failed.\n");
            return 1;
        }
    }

    WaitForMultipleObjects(4, threads, TRUE, INFINITE);

    for (i = 0; i < 4; i++)
        CloseHandle(threads[i]);

    CloseHandle(resource);
    CloseHandle(readMutex);

    printf("\nReader-Writer simulation completed.\n");
    return 0;
}
