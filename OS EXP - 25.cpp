
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    int fd;
    char path[260];
    char data[] = "Operating System Lab";
    char buffer[100] = {0};
    struct _stat fileInfo;
    long position;

    printf("Enter the full file path:\n");
    printf("Example: D:\\OS Lab\\sample25.txt\n");
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

    /* Create and open the file */
    fd = _open(path, _O_CREAT | _O_RDWR | _O_BINARY,
               _S_IREAD | _S_IWRITE);

    if (fd == -1) {
        printf("Unable to open or create file.\n");
        printf("Check the path and ensure the folder exists.\n");
        return 1;
    }

    printf("\nFile opened successfully.\n");

    /* Write data */
    if (_write(fd, data, (unsigned int)strlen(data)) == -1) {
        printf("Writing failed.\n");
        _close(fd);
        return 1;
    }

    printf("Data written successfully.\n");

    /* Demonstrate file control */
    printf("File descriptor: %d\n", fd);

    /* Move file pointer to the beginning */
    position = _lseek(fd, 0, SEEK_SET);
    printf("File pointer position: %ld\n", position);

    /* Read data */
    {
        int bytesRead = _read(fd, buffer, sizeof(buffer) - 1);

        if (bytesRead >= 0) {
            buffer[bytesRead] = '\0';
            printf("File contents: %s\n", buffer);
        } else {
            printf("Reading failed.\n");
        }
    }

    /* Get file information */
    if (_fstat(fd, &fileInfo) == 0) {
        printf("File size: %lld bytes\n",
               (long long)fileInfo.st_size);
    } else {
        printf("Unable to get file information.\n");
    }

    /* Close the file */
    _close(fd);
    printf("File closed successfully.\n");

    /* Get information using the file path */
    if (_stat(path, &fileInfo) == 0) {
        printf("\nFile information:\n");
        printf("File size: %lld bytes\n",
               (long long)fileInfo.st_size);
    }

    printf("Saved location: %s\n", path);

    return 0;
}
