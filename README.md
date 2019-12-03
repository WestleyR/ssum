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
PREFIX = ${HOME}/.local
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

### Using the shared library for other projects

This also comes with a shared library (`libssum.1.so`) that other projects can
use. By default, it is installed in `~/.lib`, and the header file is in `~/.lib/include`.
To make this accessible to your c compiler, you need to add this to your `~/.bashrc` file:

```
export CPATH=${HOME}/.local/include:${CPATH}
export LIBRARY_PATH=${HOME}/.local/lib:${LIBRARY_PATH}
```

Then you just need to add this to your c file:

```c
#include <ssum.1.h>
```

And add this to your Makefile:

```
CFLAGS += -lssum.1
```

And remember to use `CFLAGS` after the source/object files.

If you dont have your `CPATH` or `LIBRARY_PATH` set, then you can use this instead:
_But you should really be using the env vars, but this also works._

```
LDFLAGS += -L/home/westleyk/.lib -I/home/westleyk/.lib/include
```

#### Use gpack to install/manage the library (beta)

You can also use `gpack` to manage/install/update/remove the library, and
can be installed with:

```
gpack install WestleyR/ssum-lib  # or
gpack install WestleyR/lib-ssum
```

This will only install the ssum library to `~/.gpack/installed/*` and the
library and header file will be symlinked to `~/.local/lib` amd `~/.local/include`.

<br>

### Library API/functions

This library is still in beta (not released). Please see the
`lib/ssum.1.h` file for details.

<br>
