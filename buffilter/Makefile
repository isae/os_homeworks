C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib
FILES= buffilter.h buffilter.c

all: buffilter

buffilter: $(FILES)
	gcc $(C_FLAGS) -o buffilter $(FILES) -lbufio -lhelpers

clean:
	rm -f buffilter
