CC = gcc
CFLAGS = -ldl -D_GNU_SOURCE

.PHONY += all
all: build

.PHONY += build
build: build_lib build_torrify

.PHONY += build_lib
build_lib: libtorrify.so

libtorrify.so: connect.c connect.h
	$(CC) -shared -fPIC -o libtorrify.so $(CFLAGS) $<

.PHONY += build_torrify
build_torrify: torrify

torrify: torrify.c
	$(CC) -o torrify torrify.c $(CFLAGS)

.PHONY += clean
clean:
	$(RM) libtorrify.so torrify
