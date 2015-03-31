CFLAGS = -Wall -shared -fPIC -std=c99
FILES = helpers.h helpers.c
all: libhelpers.so

libhelpers.so:
	gcc $(CFLAGS) -o libhelpers.so $(FILES)

clean:
	rm -f *.so
