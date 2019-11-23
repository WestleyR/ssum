// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Nov 23, 2019
// version-1.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#define SCRIPT_VERSION "v1.0.0-beta-8, Nov 23, 2019"

#ifndef COMMIT_HASH
#define COMMIT_HASH "unknown"
#endif

void print_version() {
  printf("Version: %s\n", SCRIPT_VERSION);
}

void print_commit() {
  printf("%s\n", COMMIT_HASH);
}

void print_usage(const char* name) {
  printf("Usage: %s [option] <file(s)...>\n", name);
  printf("\n");
  printf("Compute a checksum for a ascii file.\n");
  printf("\n");
  printf("Options:\n");
  printf("  -s, --hash      generate a checksum for a file(s) (default)\n");
  printf("  -c, --check     check a files checksum\n");
  printf("  -h, --help      print help menu\n");
  printf("  -C, --commit    print the github commit hash\n");
  printf("  -V, --version   print version\n");
  printf("\n");
}

//static unsigned long gen_hash(unsigned char *message, int print) {
static unsigned long gen_hash(const char *message, int print) {
  unsigned long sum = 0;
  int c;

  while ((c = *message++) != '\0') {
    sum = c + (sum << 6) + (sum << 16) - sum;
  }

  if (print) {
    printf("%02x", (unsigned char)sum);
  }

  return(sum);
}

char* gen_checksum_file(const char* in, int print_out){
  FILE *fp_in;
  char file_line[256];

  char hsum[512];
  hsum[0] = '\0';

  int line_count = 0;

  fp_in = fopen(in, "r");
  if (!fp_in) {
    perror(in);
    printf("Unable to open file\n");
    return(NULL);
  }

  while (fgets(file_line, sizeof(file_line), fp_in)) {
    unsigned char h = gen_hash(file_line, 0);
    char s[10];
    sprintf(s, "%02x", h);
    strcat(hsum, s);
    line_count++;
  }
  fclose(fp_in);

  // Add a buffer to make hsum a known length.
  for (int i = strlen(hsum); i < 511; i++) {
    strcat(hsum, "0");
  }

#ifdef DEBUG
  printf("Unmodifyed hash: %s\n", hsum);
#endif

  char hash[256];
  hash[0] = '\0';

  static const int max_hash_len = 32;
  static const int max_block = 16;

  char block_hash[40];
  int block_index = 0;
  for (int i = 0; i < strlen(hsum); i++) {
    block_hash[block_index] = hsum[i];
    block_index++;
    if (block_index >= max_block) {
      block_hash[block_index] = '\0';
#ifdef DEBUG
      printf("Block: %s\n", block_hash);
#endif
      unsigned char h = gen_hash(block_hash, 0);
      char s[10];
      sprintf(s, "%02x", h);
      strcat(hash, s);
      block_hash[0] = '\0';
      block_index = 0;
    }
  }

  for (int i = strlen(hash); i < max_hash_len; i++) {
    strcat(hash, "0");
  }

  if (print_out) {
    printf("%s %s\n", hash, in);
  }

  char* ret_hash;
  ret_hash = (char*) malloc(256);

  strcpy(ret_hash, hash);

  return(ret_hash);
}

int check_checksum_file(const char* file) {
  FILE* fp_in = fopen(file, "r");
  if (fp_in == NULL) {
    perror(file);
    printf("Unable to open file\n");
    return(1);
  }

  int success = 0;

  char file_line[256];
  file_line[0] = '\0';

  while (fgets(file_line, sizeof(file_line), fp_in)) {
    char* hash = strtok(file_line, " ");
    char* check_file = strtok(NULL, " ");
    check_file = strtok(check_file, "\n");
    char* real_checksum = gen_checksum_file(check_file, 0);
    if (real_checksum == NULL) {
      fprintf(stderr, "Bad checksum file: %s\n", file);
      fclose(fp_in);
      return(1);
    }
    if (strcmp(real_checksum, hash) != 0) {
      printf("%s: FAIL: hashes differ\n", check_file);
      success = 1;
    } else {
      printf("%s: OK\n", check_file);
    }
#ifdef DEBUG
    printf("hash: %s\n", hash);
    printf("file: %s\n", check_file);
    printf("Real checksum: %s\n", real_checksum);
#endif
  }
  fclose(fp_in);

  return(success);
}

int main(int argc, char** argv) {
  int opt = 0;

  int checksum_file = 1;
  int check_file = 0;

  static struct option long_options[] = {
    {"hash", no_argument, 0, 's'},
    {"check", no_argument, 0, 'c'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {"commit", no_argument, 0, 'C'},
    {NULL, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "schCV", long_options, 0)) != -1) {
    switch (opt) {
      case 's':
        checksum_file = 1;
        break;

      case 'c':
        check_file = 1;
        checksum_file = 0;
        break;

      case 'V':
        print_version();
        return(0);

      case 'C':
        print_commit();
        return(0);
 
      case 'h':
        print_usage(argv[0]);
        return(0);

      default:
        return(22);
    }
  }

  int exit_code = 0;

  if (optind < argc) {
    if (argc - optind < 1) {
      fprintf(stderr, "Invallid file(s)\n");
      return(1);
    }
    for (int i = optind; i < argc; i++) {
      if (checksum_file) {
        gen_checksum_file(argv[i], 1);
      } else if (check_file) {
        if (argc - optind > 1) {
          fprintf(stderr, "Only one file allowed\n");
          return(22);
        }
        int ecode = check_checksum_file(argv[i]);
        if (ecode != 0) {
          exit_code = 1;
        }
      } else {
        fprintf(stderr, "No action specifyed\n");
        return(22);
      }
    }
  } else {
    fprintf(stderr, "No files, nothing to do...\n");
    return(123);
  }

  return(exit_code);
}

// vim: tabstop=2 shiftwidth=2 expandtab
