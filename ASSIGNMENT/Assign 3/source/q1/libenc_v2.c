/* The bank's encryption library, version 2 (security fix: key changes per byte, for demonstration only). */
#include <stddef.h>
int enc_version(void) { return 2; }
void encrypt(char *buf, size_t n) { for (size_t i = 0; i < n; i++) buf[i] ^= (char)(0xA7 + i * 31); }
