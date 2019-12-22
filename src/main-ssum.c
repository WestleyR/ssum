// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Dec 21, 2019
// version-2.0.0
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef WITHOUT_LIB
#include "ssum.1.h"
#else
#include <ssum.1.h>
#endif

#ifndef SSUM_VERSION
#define SSUM_VERSION "unknown"
#endif

#define DEFAULT_BLOCK_SIZE 20

#ifndef COMMIT_HASH
#define COMMIT_HASH "unknown"
#endif

void print_version() {
  printf("%s\n", SSUM_VERSION);
}

void print_commit() {
  printf("%s\n", COMMIT_HASH);
}

void print_usage(const char* name) {
  printf("Usage: %s [option] <file(s)...>\n", name);
  printf("\n");
  printf("Compute a CRC32 checksum for file(s).\n");
  printf("\n");
  printf("Options:\n");
  printf("  -s, --hash              generate a checksum for a file(s) (default)\n");
  printf("  -c, --check             check a files checksum from a .ssum file\n");
  printf("  -H, --file-hash <hash>  check a file with the specifyed hash\n");
  printf("  -B, --block-size <int>  use a specified block size (default: 20)\n");
  printf("  -h, --help              print help menu\n");
  printf("  -C, --commit            print the github commit hash\n");
  printf("  -V, --version           print version\n");
  printf("\n");
}

int handle_files(FILE* fp, const char* filename, int checksum_file, int check_file, int block_size) {
  if (checksum_file == check_file) {
    return(1);
  }
  int ret = 0;

  if (fp == NULL) {
    return(1);
  }

  if (checksum_file) {
    int filehash = crc32_file(fp, block_size);
    if (filehash == -1) {
      return(1);
    }
    printf("%08x %s\n", filehash, filename);
  } else if (check_file) {
    int ecode = check_crc32_file(fp, block_size);
    if (ecode != 0) {
      ret = 1;
    }
  }

  return(ret);
}

int main(int argc, char** argv) {
  int opt = 0;

  int checksum_file = 1;
  int check_file = 0;

  int block_size = DEFAULT_BLOCK_SIZE;

  static struct option long_options[] = {
    {"hash", no_argument, 0, 's'},
    {"check", no_argument, 0, 'c'},
    {"block-size", required_argument, 0, 'B'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {"commit", no_argument, 0, 'C'},
    {NULL, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "B:schCV", long_options, 0)) != -1) {
    switch (opt) {
      case 's':
        checksum_file = 1;
        break;

      case 'c':
        check_file = 1;
        checksum_file = 0;
        break;

      case 'B':
        block_size = atoi(optarg);
        if (block_size == -1 || block_size == 0) {
          fprintf(stderr, "Invalid block size\n");
          return(1);
        }
        printf("BLOCK_SIZE: %d\n", block_size);
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

  if (checksum_file && check_file) {
    fprintf(stderr, "Only one action allowed\n");
    return(22);
  }

  int exit_code = 0;

  if (optind < argc) {
    if (argc - optind < 1) {
      fprintf(stderr, "Invallid file(s)\n");
      return(1);
    }
    for (int i = optind; i < argc; i++) {
      struct stat st;
      if (stat(argv[i], &st) != 0) {
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
        return(1);
      }
      if (!S_ISREG(st.st_mode)) {
        fprintf(stderr, "%s: Is not a regular file\n", argv[i]);
        return(1);
      }

      FILE* fp = NULL;
      if (checksum_file) {
        fp = fopen(argv[i], "rb");
      } else if (check_file) {
        fp = fopen(argv[i], "r");
      }

      if (fp == NULL) {
        fprintf(stderr, "Failed to open: %s\n", argv[i]);
        return(1);
      }

      int ecode = handle_files(fp, argv[i], checksum_file, check_file, block_size);
      if (ecode != 0) {
        exit_code = 1;
      }
      fclose(fp);
    }
  } else {
    int ecode = handle_files(stdin, "-", checksum_file, check_file, block_size);
    return(ecode);
  }

  return(exit_code);
}

// vim: tabstop=2 shiftwidth=2 expandtab
