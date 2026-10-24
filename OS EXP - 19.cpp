
#include <windows.h>
#include <stdio.h>

HANDLE mutex;

DWORD WINAPI worker(LPVOID arg)
{
    int id = *(int *)arg;
    int i;

    for (i = 1; i <= 3; i++) {
        WaitForSingleObject(mutex, INFINITE);

        printf("Thread %d entered critical section.\n", id);
        Sleep(300);
        printf("Thread %d leaving critical section.\n", id);

        ReleaseMutex(mutex);
        Sleep(100);
    }

    return 0;
}

int main(void)
{
    HANDLE t1, t2;
    int id1 = 1, id2 = 2;

    mutex = CreateMutex(NULL, FALSE, NULL);

    if (mutex == NULL) {
        printf("Mutex creation failed.\n");
        return 1;
    }

    t1 = CreateThread(NULL, 0, worker, &id1, 0, NULL);
    t2 = CreateThread(NULL, 0, worker, &id2, 0, NULL);

    if (t1 == NULL || t2 == NULL) {
        printf("Thread creation failed.\n");
        return 1;
    }

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);

    CloseHandle(t1);
    CloseHandle(t2);
    CloseHandle(mutex);

    printf("\nAll threads completed.\n");
    return 0;
}
