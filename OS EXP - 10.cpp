#include <stdio.h>
#include <windows.h>

int main()
{
    DWORD threadID;
    MSG msg;
    char message[] = "Hello from Sender Process!";

    printf("MESSAGE QUEUE IPC\n");
    printf("------------------\n");

    /* Get current thread ID */
    threadID = GetCurrentThreadId();

    printf("Sender: Sending message...\n");

    /* Send message to the current thread queue */
    if (PostThreadMessage(threadID, WM_USER + 1, 0, (LPARAM)message))
    {
        printf("Sender: Message sent successfully.\n");
    }
    else
    {
        printf("Message sending failed.\n");
        return 1;
    }

    /* Receive message */
    if (GetMessage(&msg, NULL, WM_USER + 1, WM_USER + 1))
    {
        printf("Receiver: Message received: %s\n",
               (char *)msg.lParam);
    }

    return 0;
}