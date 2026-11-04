
#include <windows.h>
#include <stdio.h>
#include <string.h>

int main() {
    HANDLE file;
    DWORD written, bytesRead;
    char path[MAX_PATH];
    char text[] = "Hello, World!\r\n";
    char buffer[100] = {0};

    printf("Enter the full file path to save the file.\n");
    printf("Example: D:\\OS Lab\\sample.txt\n");
    printf("File path: ");

    if (fgets(path, sizeof(path), stdin) == NULL) {
        printf("Invalid input!\n");
        return 1;
    }

    path[strcspn(path, "\r\n")] = '\0';

    if (strlen(path) == 0) {
        printf("File path cannot be empty!\n");
        return 1;
    }

    file = CreateFileA(
        path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        printf("File creation failed! Error code: %lu\n",
               GetLastError());
        printf("Check that the folder exists and the path is valid.\n");
        return 1;
    }

    printf("File created successfully.\n");

    if (!WriteFile(file, text, sizeof(text) - 1,
                   &written, NULL)) {
        printf("Writing failed!\n");
        CloseHandle(file);
        return 1;
    }

    printf("Data written successfully.\n");
    CloseHandle(file);

    file = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        printf("File opening failed!\n");
        return 1;
    }

    if (ReadFile(file, buffer, sizeof(buffer) - 1,
                 &bytesRead, NULL)) {
        buffer[bytesRead] = '\0';
        printf("File contents: %s", buffer);
    } else {
        printf("Reading failed!\n");
    }

    CloseHandle(file);

    printf("\nFile closed successfully.\n");
    printf("Saved location: %s\n", path);

    return 0;
}
