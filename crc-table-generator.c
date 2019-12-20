//
// This will generate a crc table with the given polynomial, in this
// case, thats: 0x04c11db7.
//
// This file is NOT used by ssum or any ssum libraries.
//

#include <stdio.h>

int main() {
  unsigned int i, j;
  unsigned int c;
  int line_count = 0;
  int table[256];
  for (i = 0; i < 256; i++) {
    for (c = i << 24, j = 8; j > 0; --j) {
      c = c & 0x80000000 ? (c << 1) ^ 0x04c11db7 : (c << 1);
    }
    table[i] = c;
  }
  for (i = 0; i < 256; i++) {
    printf("0x%08x, ", table[i]);
    line_count++;
    if (line_count == 5) {
      printf("\n");
      line_count = 0;
    }
  }
  printf("\n");

  return 0;
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
