/* Segmentation: translate (segment, offset) with a segment table of base and limit; detect limit violations. */
#include <stdio.h>
struct seg { const char *name; unsigned base, limit; const char *perm; };
struct seg table[] = {
    {"code ", 1400, 1000, "r-x"},
    {"data ", 6300,  400, "rw-"},
    {"heap ", 4300,  400, "rw-"},
    {"stack", 3200, 1100, "rw-"},
    {"lib  ", 4700, 1000, "r-x"},
};
int main(void)
{
    struct { int s; unsigned off; char op; } t[] = {
        {0, 53, 'x'}, {1, 852, 'r'}, {2, 399, 'w'}, {3, 1099, 'w'}, {4, 0, 'x'}, {0, 1000, 'x'}, {1, 100, 'x'}, {5, 10, 'r'}};
    printf("%-3s %-6s %-5s %-5s  result\n", "#", "seg", "off", "op");
    for (int i = 0; i < 8; i++) {
        printf("%-3d ", i + 1);
        if (t[i].s >= 5) { printf("%-6d %-5u %-5c  TRAP: invalid segment number\n", t[i].s, t[i].off, t[i].op); continue; }
        struct seg *g = &table[t[i].s];
        printf("%-6s %-5u %-5c  ", g->name, t[i].off, t[i].op);
        if (t[i].off >= g->limit) { printf("TRAP: offset %u >= limit %u (segmentation fault)\n", t[i].off, g->limit); continue; }
        char need = t[i].op;
        int ok = need == 'r' ? g->perm[0] == 'r' : need == 'w' ? g->perm[1] == 'w' : g->perm[2] == 'x';
        if (!ok) { printf("TRAP: protection violation (segment is %s)\n", g->perm); continue; }
        printf("physical %u + %u = %u\n", g->base, t[i].off, g->base + t[i].off);
    }
    return 0;
}
