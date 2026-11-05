/* The bank's encryption library, version 1 (simple XOR cipher, for demonstration only). */
#include <stddef.h>
int enc_version(void) { return 1; }
void encrypt(char *buf, size_t n) { for (size_t i = 0; i < n; i++) buf[i] ^= 0x5A; }
