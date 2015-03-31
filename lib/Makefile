CFLAGS = -Wall -shared -fPIC -std=c99
DEBUG_FLAGS= 
BUFIO_FILES = bufio.h bufio.c
all: libbufio.so

debug: DEBUG_FLAGS= -DDEBUG -g
debug: all

libbufio.so: $(BUFIO_FILES)
	gcc $(DEBUG_FLAGS) $(CFLAGS) -o libbufio.so $(BUFIO_FILES)

clean:
	rm -f *.so
