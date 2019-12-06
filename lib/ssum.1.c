// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Nov 24, 2019
// version-2.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#include "ssum.1.h"

int gen_hash(const char *msg) {
  uint8_t i;

  // The init value.
  uint16_t hash = 0x1d0f;

  // The magic polynormial for CRC-CCITT.
  static const uint16_t CRC_POLY = 0x1021;

  int len = strlen(msg);

  while (len-- >= 0) {
    hash = hash ^ ((uint16_t)(*msg++ << 8));
    for (i = 0; i < 8; i++) {
      if (hash & 0x8000) {
        hash = (hash << 1) ^ CRC_POLY;
      } else {
        hash = hash << 1;
      }
    }
  }

  return(hash);
}

int gen_checksum_file(const char* in) {
  static const int block_size = 100;

  FILE *fp_in;
  fp_in = fopen(in, "r");
  if (fp_in == NULL) {
    perror(in);
    fprintf(stderr, "Unable to open file: %s\n", in);
    return(0);
  }

  int line_count = 0;
  char line[256];
  int c = 0;

  int hsum = 0;

  while (c != EOF) {
    c = fgetc(fp_in); 
    if (c == EOF) {
      line[line_count] = '\0';
      int h = gen_hash(line);
      hsum += h;
      break;
    }

    line[line_count] = c;
    line_count++;

    if (line_count >= block_size) {
      line[line_count] = '\0';
#ifdef DEBUG
      printf("Block: %s\n", line);
#endif
      int h = gen_hash(line);
      hsum += h;

      line[0] = '\0';
      line_count = 0;
    }
  }
  fclose(fp_in);

#ifdef DEBUG
  printf("Int hash: %d\n", hsum);
#endif

  return(hsum);
}

int check_checksum_file(const char* file) {
  FILE* fp_in = fopen(file, "r");
  if (fp_in == NULL) {
    perror(file);
    fprintf(stderr, "Unable to open file: %s\n", file);
    return(1);
  }

  int success = 0;

  char file_line[256];
  file_line[0] = '\0';

  while (fgets(file_line, sizeof(file_line), fp_in)) {
    char* hash = strtok(file_line, " ");
    char* check_file = strtok(NULL, " ");
    check_file = strtok(check_file, "\n");
    int real_checksum = gen_checksum_file(check_file);
    if (real_checksum == -1) {
      fprintf(stderr, "Bad checksum file: %s\n", file);
      fclose(fp_in);
      return(1);
    }
    int num_hash = (int)strtol(hash, NULL, 16);
    if (real_checksum != num_hash) {
      printf("%s: FAIL: hashes differ\n", check_file);
      success = 1;
    } else {
      printf("%s: OK\n", check_file);
    }
#ifdef DEBUG
    printf("hash: %d\n", num_hash);
    printf("file: %s\n", check_file);
    printf("Real checksum: %d\n", real_checksum);
#endif
  }
  fclose(fp_in);

  return(success);
}

// vim: tabstop=2 shiftwidth=2 expandtab
