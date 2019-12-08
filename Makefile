# Created by: WestleyR
# email: westleyr@nym.hush.com
# Date: Dec 7, 2019
# https://github.com/WestleyR/ssum
# Version-1.0.2
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

LDFLAGS ?= -L$(PREFIX)/lib -I$(PREFIX)/include

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
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -lssum.1 -o $(TARGET)

.PHONY:
install-lib: $(LIB)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp -f $(LIB) $(PREFIX)/lib
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
	$(CC) -c -fPIC lib/ssum.1.c -Iinclude -o lib/ssum.1.o
	$(CC) -shared -o $(LIB) lib/ssum.1.o

.PHONY:
without-lib: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -Iinclude -DWITHOUT_LIB -o $(TARGET) lib/ssum.1.c

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
