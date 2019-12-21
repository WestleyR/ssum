# Created by: WestleyR
# email: westleyr@nym.hush.com
# Date: Dec 20, 2019
# https://github.com/WestleyR/ssum
# Version-1.1.2
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

# Default prefix
PREFIX = $(HOME)/.local

SRC = $(wildcard src/*.c)
LIB_SRC = $(wildcard lib/*.c)

OBJS = $(SRC:.c=.o)

LIB_OBJS = $(LIB_SRC:.c=.o)

COMMIT = "$(shell git log -1 --oneline --decorate=short --no-color || ( echo 'ERROR: unable to get commit hash' >&2 ; echo unknown ) )"
COMMIT_FLAG = -DCOMMIT_HASH=\"$(COMMIT)\"

ifeq ($(DEBUG), true)
	CFLAGS += -DDEBUG
endif

LDFLAGS = 

ifeq ($(RPATH), true)
	LDFLAGS += -Wl,-rpath=$(PREFIX)/lib
endif
ifeq ($(CPATH),)
	LDFLAGS += -I$(PREFIX)/include
endif
ifeq ($(LIBRARY_PATH),)
	LDFLAGS += -L$(PREFIX)/lib
endif

.PHONY:
all: $(TARGET)

.PHONY:
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) src/main-ssum.c -lssum.1 -o $(TARGET) $(LDFLAGS)

.PHONY:
install-lib: $(LIB)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp -f lib/$(LIB) $(PREFIX)/lib
	cp -f include/ssum.1.h $(PREFIX)/include
	@echo
	@echo "Make sure you add this to your .bashrc: (so other projects can use this library)"
	@echo
	@echo "export LD_LIBRARY_PATH=\$${HOME}/local/.lib/:\$${LD_LIBRARY_PATH}"
	@echo "export CPATH=\$${HOME}/.local/include:\$${CPATH}"
	@echo "export LIBRARY_PATH=\$${HOME}/.local/lib:\$${LIBRARY_PATH}"
	@echo

.PHONY:
$(LIB):
	$(MAKE) -C ./lib shared-library

.PHONY:
$(LIB_OBJS):
	$(MAKE) -C ./lib objs

.PHONY:
without-lib: $(OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_OBJS) -DWITHOUT_LIB -o $(TARGET) $(LDFLAGS)

.PHONY:
%.o: %.c
	$(CC) $(CFLAGS) $(COMMIT_FLAG) -I./include -o $@ -c $< $(LDFLAGS)

.PHONY:
test: $(TARGET)
	@bash ./run-tests

.PHONY:
install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

.PHONY:
clean:
	rm -f $(TARGET) $(OBJS)
	$(MAKE) -C ./lib clean

.PHONY:
uninstall: $(PREFIX)/$(TARGET)
	rm -f $(PREFIX)/$(TARGET)

#
# End Makefile
#
