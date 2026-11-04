#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int source, destination;
    char sourceFile[200];
    char destinationFile[200];
    char buffer[1024];
    int bytesRead;

    /* Get source file */
    printf("Enter source file name: ");
    fgets(sourceFile, sizeof(sourceFile), stdin);
    sourceFile[strcspn(sourceFile, "\n")] = '\0';

    /* Open source file */
    source = open(sourceFile, O_RDONLY);

    if (source < 0)
    {
        printf("Error: Cannot open source file.\n");
        return 1;
    }

    /* Get destination file */
    printf("Enter destination file name: ");
    fgets(destinationFile, sizeof(destinationFile), stdin);
    destinationFile[strcspn(destinationFile, "\n")] = '\0';

    /* Create destination file */
    destination = open(destinationFile,
                       O_WRONLY | O_CREAT | O_TRUNC,
                       0644);

    if (destination < 0)
    {
        printf("Error: Cannot create destination file.\n");
        close(source);
        return 1;
    }

    /* Copy contents */
    while ((bytesRead = read(source, buffer, sizeof(buffer))) > 0)
    {
        write(destination, buffer, bytesRead);
    }

    close(source);
    close(destination);

    printf("File copied successfully.\n");

    return 0;
}