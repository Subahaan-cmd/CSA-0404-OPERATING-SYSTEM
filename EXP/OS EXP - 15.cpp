
#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <io.h>
#include <sys/stat.h>

int main(void)
{
    char user[30], filename[100], path[200];
    int choice;
    FILE *fp;
    struct _finddata_t entry;
    intptr_t handle;

    do {
        printf("\n--- TWO LEVEL DIRECTORY ---\n");
        printf("1. Create User Directory\n");
        printf("2. Create File\n");
        printf("3. Search File\n");
        printf("4. List Files\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            return 1;

        if (choice == 1) {
            printf("Enter user name: ");
            scanf("%29s", user);

            if (_mkdir(user) == 0)
                printf("User directory created: %s\n", user);
            else {
                struct stat st;

                if (stat(user, &st) == 0 && (st.st_mode & S_IFDIR))
                    printf("User directory already exists: %s\n", user);
                else
                    printf("Unable to create directory.\n");
            }
        }
        else if (choice == 2) {
            printf("Enter user name: ");
            scanf("%29s", user);

            if (_access(user, 0) != 0) {
                printf("User directory does not exist. Create it first.\n");
                continue;
            }

            printf("Enter file name: ");
            scanf("%99s", filename);

            snprintf(path, sizeof(path), "%s\\%s", user, filename);

            fp = fopen(path, "w");

            if (fp == NULL) {
                printf("File creation failed.\n");
            } else {
                fclose(fp);
                printf("File created successfully: %s\n", path);
            }
        }
        else if (choice == 3) {
            printf("Enter user name: ");
            scanf("%29s", user);
            printf("Enter file name: ");
            scanf("%99s", filename);

            snprintf(path, sizeof(path), "%s\\%s", user, filename);
            fp = fopen(path, "r");

            if (fp != NULL) {
                printf("File found: %s\n", path);
                fclose(fp);
            } else {
                printf("File not found.\n");
            }
        }
        else if (choice == 4) {
            printf("Enter user name: ");
            scanf("%29s", user);

            snprintf(path, sizeof(path), "%s\\*.*", user);
            handle = _findfirst(path, &entry);

            if (handle == -1) {
                printf("No files found, or directory does not exist.\n");
            } else {
                printf("Files in %s:\n", user);

                do {
                    if (!(entry.attrib & _A_SUBDIR))
                        printf("%s\n", entry.name);
                } while (_findnext(handle, &entry) == 0);

                _findclose(handle);
            }
        }
        else if (choice != 5) {
            printf("Invalid choice.\n");
        }

    } while (choice != 5);

    printf("Program terminated.\n");
    return 0;
}
