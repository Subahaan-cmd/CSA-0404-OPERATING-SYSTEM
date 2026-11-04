
#include <windows.h>
#include <stdio.h>

#define SIZE 5
#define ITEMS 10

int buffer[SIZE];
int in = 0, out = 0;

HANDLE emptySlots, fullSlots, mutex;

DWORD WINAPI producer(LPVOID arg)
{
    int i;

    for (i = 1; i <= ITEMS; i++) {
        WaitForSingleObject(emptySlots, INFINITE);
        WaitForSingleObject(mutex, INFINITE);

        buffer[in] = i;
        printf("Produced: %d\n", i);
        in = (in + 1) % SIZE;

        ReleaseSemaphore(mutex, 1, NULL);
        ReleaseSemaphore(fullSlots, 1, NULL);

        Sleep(100);
    }

    return 0;
}

DWORD WINAPI consumer(LPVOID arg)
{
    int i, item;

    for (i = 1; i <= ITEMS; i++) {
        WaitForSingleObject(fullSlots, INFINITE);
        WaitForSingleObject(mutex, INFINITE);

        item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % SIZE;

        ReleaseSemaphore(mutex, 1, NULL);
        ReleaseSemaphore(emptySlots, 1, NULL);

        Sleep(150);
    }

    return 0;
}

int main(void)
{
    HANDLE p, c;

    emptySlots = CreateSemaphore(NULL, SIZE, SIZE, NULL);
    fullSlots = CreateSemaphore(NULL, 0, SIZE, NULL);
    mutex = CreateSemaphore(NULL, 1, 1, NULL);

    if (emptySlots == NULL || fullSlots == NULL ||
        mutex == NULL) {
        printf("Semaphore creation failed.\n");
        return 1;
    }

    p = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    c = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    if (p == NULL || c == NULL) {
        printf("Thread creation failed.\n");
        return 1;
    }

    WaitForSingleObject(p, INFINITE);
    WaitForSingleObject(c, INFINITE);

    CloseHandle(p);
    CloseHandle(c);
    CloseHandle(emptySlots);
    CloseHandle(fullSlots);
    CloseHandle(mutex);

    printf("\nProducer-Consumer completed.\n");
    return 0;
}
