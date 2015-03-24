C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib

all: clean filter

filter:
	gcc $(C_FLAGS) -o filter filter.c -lhelpers

clean:
	rm -f filter
