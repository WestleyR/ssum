# Simple checksum program for ascii files

![](https://github.com/WestleyR/ssum/workflows/ssum%20CI/badge.svg)

This is a simple example of a checksum program.

## Install

Clone the source code:

```
git clone https://github.com/WestleyR/ssum
cd ssum/
make
sudo make install
# Or add the 'ssum' binary to your perfured path
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

