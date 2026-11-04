
#include <windows.h>
#include <stdio.h>

DWORD WINAPI worker(LPVOID arg)
{
    printf("Thread %s is running\n", (char *)arg);
    return 0;
}

int main(void)
{
    HANDLE h[2];

    h[0] = CreateThread(NULL, 0, worker, (LPVOID)"A", 0, NULL);
    h[1] = CreateThread(NULL, 0, worker, (LPVOID)"B", 0, NULL);

    if (h[0] == NULL || h[1] == NULL)
    {
        printf("Thread creation failed\n");
        return 1;
    }

    WaitForMultipleObjects(2, h, TRUE, INFINITE);

    CloseHandle(h[0]);
    CloseHandle(h[1]);

    printf("Both threads completed\n");
    return 0;
}
