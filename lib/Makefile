CFLAGS = -Wall -shared -fPIC -std=c99
all: libhelpers.so

libhelpers.so:
	gcc $(CFLAGS) -o libhelpers.so helpers.h helpers.c

clean:
	rm -f *.so
