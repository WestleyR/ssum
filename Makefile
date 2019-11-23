# Created by: WestleyR
# email: westleyr@nym.hush.com
# Date: Nov 23, 2019
# https://github.com/WestleyR/ssum
# Version-1.0.0
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

PREFIX = /usr/local

MAIN = src/main-binaryfile.c

.PHONY:
all: $(TARGET)

.PHONY:
$(TARGET): $(MAIN)
	$(CC) $(CFLAGS) $(MAIN) -o $(TARGET)

.PHONY:
static: $(MAIN)
	$(CC) $(CFLAGS) -static -o $(TARGET) $(MAIN)

.PHONY:
test: $(TARGET)
	@bash ./run-tests

.PHONY:
install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

.PHONY:
clean:
	 rm -f $(TARGET)

.PHONY:
uninstall: $(PREFIX)/$(TARGET)
	rm -f $(PREFIX)/$(TARGET)


# vim: tabstop=4 noexpandtab 

