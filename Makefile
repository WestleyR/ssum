# Created by: WestleyR
# email: westleyr@nym.hush.com
# Date: Nov 23, 2019
# https://github.com/WestleyR/ssum
# Version-1.0.1
#
# The Clear BSD License
#
# Copyright (c) 2019 WestleyR
# All rights reserved.
#
# This software is licensed under a Clear BSD License.
#

CC = gcc
CFLAGS = -Wall

TARGET = ssum
LIB = libssum.1.so

# Binary prefixes
PREFIX = $(HOME)/.local/bin
LIB_PREFIX = $(HOME)/.lib
INCLUDE_PREFIX = $(HOME)/.lib/include

SRC = src/main-ssum.c

COMMIT = "$(shell git log -1 --oneline --decorate=short --no-color || ( echo 'ERROR: unable to get commit hash' >&2 ; echo unknown ) )"

CFLAGS += -DCOMMIT_HASH=\"$(COMMIT)\"

ifeq ($(DEBUG), true)
	CFLAGS += -DDEBUG
endif

.PHONY:
all: $(TARGET)

.PHONY:
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -L$(HOME)/.lib -I$(HOME)/.lib/include -lssum.1 -o $(TARGET) $(SRC)

.PHONY:
install-lib: $(LIB)
	mkdir -p $(LIB_PREFIX)
	mkdir -p $(INCLUDE_PREFIX)
	cp -f $(LIB) $(LIB_PREFIX)
	cp -f lib/ssum.1.h $(INCLUDE_PREFIX)

.PHONY:
$(LIB):
	$(CC) -c -fPIC lib/ssum.1.c -o lib/ssum.1.o
	$(CC) -shared -o $(LIB) lib/ssum.1.o

.PHONY:
without-lib: $(SRC)
	$(CC) $(CFLAGS) -Ilib -DWITHOUT_LIB -o $(TARGET) $(SRC) lib/ssum.1.c

.PHONY:
test: $(TARGET)
	@bash ./run-tests

.PHONY:
install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

.PHONY:
clean:
	 rm -f $(TARGET) $(LIB) lib/ssum.1.o

.PHONY:
uninstall: $(PREFIX)/$(TARGET)
	rm -f $(PREFIX)/$(TARGET)

#
# End Makefile
#
