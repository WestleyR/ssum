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

#define SCRIPT_VERSION "v2.0.1, Dec 21, 2019"

#ifndef COMMIT_HASH
#define COMMIT_HASH "unknown"
#endif

void print_version() {
  printf("lib-ssum version: %s; ssum version: %s\n", libssum_version(), SCRIPT_VERSION);
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
  printf("  -h, --help              print help menu\n");
  printf("  -C, --commit            print the github commit hash\n");
  printf("  -V, --version           print version\n");
  printf("\n");
}

int handle_files(const char* file, int checksum_file, int check_file) {
  if (checksum_file == check_file) {
    return(1);
  }
  int ret = 0;

  struct stat st;
  stat(file, &st);
  if (!S_ISREG(st.st_mode)) {
    fprintf(stderr, "%s: Is not a regular file\n", file);
    return(1);
  }

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
    printf("%08x %s\n", filehash, file);
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

  int check_file_hash = 0;
  char* checkfile_hash;

  static struct option long_options[] = {
    {"hash", no_argument, 0, 's'},
    {"check", no_argument, 0, 'c'},
    {"file-hash", required_argument, 0, 'H'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {"commit", no_argument, 0, 'C'},
    {NULL, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "H:schCV", long_options, 0)) != -1) {
    switch (opt) {
      case 's':
        checksum_file = 1;
        break;

      case 'c':
        check_file = 1;
        checksum_file = 0;
        break;

      case 'H':
        checkfile_hash = (char*) malloc(36*sizeof(char));
        if (checkfile_hash == NULL) {
          perror("malloc");
          return(1);
        }
        checkfile_hash = optarg;
        check_file_hash = 1;
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

  if (check_file_hash) {
    int num_hash = (int)strtol(checkfile_hash, NULL, 16);
    if (optind < argc) {
      if (argc - optind > 1) {
        fprintf(stderr, "Invallid file(s)\n");
        return(1);
      }
      FILE* fp = fopen(argv[optind], "rb");
      if (fp == NULL) {
        fprintf(stderr, "%s: %s: No such file\n", argv[0], argv[optind]);
        return(1);
      }
      int file_checksum = crc32_file(fp);
      fclose(fp);
      if (file_checksum == -1) {
        fprintf(stderr, "Failed to gen hash\n");
        return(1);
      }

#ifdef DEBUG
      printf("HASH: %s\n", checkfile_hash);
      printf("hash: %x\n", file_checksum);
      printf("FILE: %s\n", argv[optind]);
#endif
      if (num_hash != file_checksum) {
        printf("%s: Hashes differ\n", argv[optind]);
        return(1);
      } else {
        printf("%s: OK\n", argv[optind]);
        return(0);
      }
    } else {
      fprintf(stderr, "No files, nothing to do...\n");
      return(123);
    }

    return(0);
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
