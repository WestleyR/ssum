// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Dec 7, 2019
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

#ifdef WITHOUT_LIB
#include "ssum.1.h"
#else
#include <ssum.1.h>
#endif

#define SCRIPT_VERSION "v2.0.0-beta-22, Dec 7, 2019"

#ifndef COMMIT_HASH
#define COMMIT_HASH "unknown"
#endif

void print_version() {
  printf("lib-ssum version: %s; ssum version: %s\n", SSUM_LIB_VERSION, SCRIPT_VERSION);
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
  printf("  -s, --hash      generate a checksum for a file(s) (default)\n");
  printf("  -c, --check     check a files checksum\n");
  printf("  -h, --help      print help menu\n");
  printf("  -C, --commit    print the github commit hash\n");
  printf("  -V, --version   print version\n");
  printf("\n");
}

int handle_files(const char* file, int checksum_file, int check_file) {
  if (checksum_file == check_file) {
    return(1);
  }
  int ret = 0;

  FILE* fp;
  if (checksum_file) {
    fp = fopen(file, "rb");
  } else if (check_file) {
    fp = fopen(file, "rb");
  }

  if (fp == NULL) {
    fprintf(stderr, "Failed to open: %s\n", file);
    return(1);
  }

  if (checksum_file) {
    int filehash = crc32_file(fp);
    if (filehash == -1) {
      return(1);
    }
    printf("%x %s\n", filehash, file);
  } else if (check_file) {
    int ecode = check_crc32_file(fp);
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
      int ecode = handle_files(argv[i], checksum_file, check_file);
      if (ecode != 0) {
        exit_code = 1;
      }
    }
  } else {
    fprintf(stderr, "No files, nothing to do...\n");
    return(123);
  }

  return(exit_code);
}

// vim: tabstop=2 shiftwidth=2 expandtab
