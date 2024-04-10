# Torrify

This project is a command line application that allows you to run any command line in a tor network.

> Note: Only Socks4 proxies supported.

## Requirments:

- tor proxy server running locally on port **9050**.

## Build

Run `make` to build the torrify binary and the libtorrify library.

## Usage

> Note: the libtorrify.so library must be in the same directory as the torrify binary.

- `torrify <command> <...args>`

### Examples:

- Run any command with arguments

```shell
$ torrify ls -1
connect.c
connect.h
libtorrify.so
makefile
README.md
torrify
torrify.c
```

- Fetch your ip address using **ipify.org** without tor proxy.

```shell
$ curl "http://api.ipify.org"
102.184.197.136 # dummy ip result without tor
```

- Fetch your ip address using **ipify.org** with tor proxy.

```shell
$ ./torrify curl "http://api.ipify.org"
188.172.229.15 # dummy ip result with tor
```
