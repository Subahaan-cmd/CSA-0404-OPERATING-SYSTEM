#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char line[256];
    if (argc != 3) { printf("Usage: %s pattern file\n", argv[0]); return 1; }

    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL) { perror("fopen"); return 1; }

    while (fgets(line, sizeof(line), fp))
        if (strstr(line, argv[1]))
            printf("%s", line);

    fclose(fp);
    return 0;
}