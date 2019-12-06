// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Nov 29, 2019
// version-1.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#ifndef SSUM_H
#define SSUM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define SSUM_LIB_VERSION "v2.0.0-beta-20, Dec 6, 2019"

int gen_hash(const char *msg);
int gen_checksum_file(const char* in);
int check_checksum_file(const char* file);

#endif // SSUM_H

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

