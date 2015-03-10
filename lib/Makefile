CFLAGS = -Wall -shared -fPIC
all: libhelpers.so

libhelpers.so:
	gcc $(CFLAGS) -o libhelpers.so helpers.h helpers.c

clean:
	rm -f *.so
