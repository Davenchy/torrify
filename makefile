CC = gcc
CFLAGS = -ldl -D_GNU_SOURCE

.PHONY += build_lib
build_lib: libtorrify.so

libtorrify.so: connect.c connect.h
	$(CC) -shared -fPIC -o libtorrify.so $(CFLAGS) $<

.PHONY += clean
clean:
	$(RM) libtorrify.so
