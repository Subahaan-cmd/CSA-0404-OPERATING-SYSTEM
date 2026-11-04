#include <stdio.h>

int main()
{
    char line[100];
    FILE *fp = fopen("F:/SUB/NOTES.txt", "w");          /* create + write */
    fprintf(fp, "Hello\n");
    fclose(fp);

    fp = fopen("F:/SUB/NOTES.txt", "a");                /* append */
    fprintf(fp, "World\n");
    fclose(fp);

    fp = fopen("F:/SUB/NOTES.txt", "r");                /* read */
    while (fgets(line, sizeof(line), fp))
        printf("%s", line);
    fclose(fp);

    rename("F:/SUB/NOTES.txt", "F:/SUB/ROUGH.txt");                /* rename */
    printf("Done\n");
    return 0;
}