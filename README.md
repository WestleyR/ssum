# Simple checksum program for ascii files

![](https://github.com/WestleyR/ssum/workflows/ssum%20CI/badge.svg)

This is a simple example of a checksum program.

## Install

Clone the source code:

```
git clone https://github.com/WestleyR/ssum
cd ssum/
make install-lib  # install the ssum.1.so library
make              # compile ssum
make install      # install ssum
```

Also, make sure you have added this to your `~/.bashrc`:

```
export LD_LIBRARY_PATH=${HOME}/.lib/:${LD_LIBRARY_PATH}
export PATH=${PATH}:${HOME}/.local/bin/
```

The `LD_LIBRARY_PATH` is needed because ssum creates a shared library, and that
is linked to the ssum command. And the `PATH` is so you dont need `sudo` to
install it.

Alternatively you can compile ssum without the shared library:

```
make without-lib
make install
```

Default prefixes:

```
PREFIX = $(HOME)/.local/bin
LIB_PREFIX = $(HOME)/.lib
INCLUDE_PREFIX = $(HOME)/.lib/include
```

<br>

## Example

```
$ ssum README.md
3ecdb8424e54e6e9dff4 README.md


$ ssum README.md Makefile > foo.ssum

$ ssum -c foo.ssum
README.md: OK
Makefile: OK
```

<br>

