C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib
FILES= filter.h filter.c

all: filter

filter: $(FILES)
	gcc $(C_FLAGS) -o filter $(FILES) -lhelpers

clean:
	rm -f filter
