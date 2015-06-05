CFLAGS = -Wall -shared -fPIC -std=c99
DEBUG_FLAGS= 
HELPERS_FILES = helpers.h helpers.c
BUFIO_FILES = bufio.h bufio.c
all: libbufio.so libhelpers.so

debug: DEBUG_FLAGS= -DDEBUG -g
debug: all

libhelpers.so: $(HELPERS_FILES)
	gcc $(DEBUG_FLAGS) $(CFLAGS) -o libhelpers.so $(HELPERS_FILES)

libbufio.so: $(BUFIO_FILES)
	gcc $(DEBUG_FLAGS) $(CFLAGS) -o libbufio.so $(BUFIO_FILES)

clean:
	rm -f *.so
