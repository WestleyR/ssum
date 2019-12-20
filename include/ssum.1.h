// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/ssum
// date: Dec 19, 2019
// version: na; see ssum.1.c
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#ifndef INCLUDE_SSUM_H
#define INCLUDE_SSUM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* libssum_version();
unsigned int crc32_hash(const unsigned char *message, int msg_len);
unsigned int crc32_file(FILE* fp);
int check_crc32_file(FILE* fp);

#endif // INCLUDE_SSUM_H

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0

