/* Task 5 alternative: the same copy loop with fread/fwrite - the library buffers 4096 bytes. */
#include <stdio.h>
int main(void)
{
    FILE *f = fopen("in.dat", "w");
    for (int i = 0; i < 10000; i++) fputc('A' + i % 26, f);
    fclose(f);
    FILE *in = fopen("in.dat", "r"), *out = fopen("out.dat", "w");
    int c;
    while ((c = fgetc(in)) != EOF) fputc(c, out);    /* 10,000 library calls */
    fclose(in); fclose(out);
    return 0;
}
