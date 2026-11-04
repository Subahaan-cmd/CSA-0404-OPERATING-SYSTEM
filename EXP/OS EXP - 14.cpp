
#include <stdio.h>
#include <string.h>

int main(void)
{
    char files[50][100], name[100];
    int n = 0, choice, i, found;

    do {
        printf("\n--- SINGLE LEVEL DIRECTORY ---\n");
        printf("1. Create File\n");
        printf("2. Search File\n");
        printf("3. List Files\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            return 1;

        if (choice == 1)
{
    FILE *fp;

    printf("Enter file name: ");
    scanf("%99s", name);

    fp = fopen(name, "r");

    if (fp != NULL)
    {
        printf("File already exists.\n");
        fclose(fp);
    }
    else
    {
        fp = fopen(name, "w");

        if (fp == NULL)
            printf("Unable to create file.\n");
        else
        {
            fclose(fp);
            strcpy(files[n++], name);
            printf("File created successfully.\n");
        }
    }
}
        else if (choice == 2) {
            printf("Enter file name to search: ");
            scanf("%99s", name);

            found = 0;
            for (i = 0; i < n; i++) {
                if (strcmp(files[i], name) == 0)
                    found = 1;
            }

            if (found)
                printf("File found.\n");
            else
                printf("File not found.\n");
        }
        else if (choice == 3) {
            if (n == 0)
                printf("Directory is empty.\n");
            else {
                printf("\nFiles in directory:\n");
                for (i = 0; i < n; i++)
                    printf("%d. %s\n", i + 1, files[i]);
            }
        }
        else if (choice != 4) {
            printf("Invalid choice.\n");
        }

    } while (choice != 4);

    printf("Program terminated.\n");
    return 0;
}
