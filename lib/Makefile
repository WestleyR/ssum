# Created by: WestleyR
# Email: westleyr@nym.hush.com
# Url: https://github.com/WestleyR/ssum
# Last modified date: 2020-05-15
#
# This file is licensed under the terms of
#
# The Clear BSD License
#
# Copyright (c) 2019-2020 WestleyR
# All rights reserved.
#
# This software is licensed under a Clear BSD License.
#

CC = gcc
CFLAGS = -Wall

TARGET = libssum.1.so

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

ifeq ($(DEBUG), true)
	CFLAGS += -DDEBUG
endif

shared-library: CFLAGS += -fPIC

.PHONY:
all: shared-library

.PHONY:
shared-library: $(OBJS)
	$(CC) $(CFLAGS) -shared ssum.1.o -o $(TARGET)

.PHONY:
objs: $(OBJS)

.PHONY:
%.o: %.c
	$(CC) $(CFLAGS) -I../include -o $@ -c $< $(LDFLAGS)

.PHONY:
clean:
	 rm -f $(TARGET) ssum.1.o

#
# End Makefile
#
