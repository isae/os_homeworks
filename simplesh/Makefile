C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib
FILES= simplesh.c

all: simplesh

simplesh: $(FILES)
	gcc $(C_FLAGS) -o simplesh $(FILES) -lbufio -lhelpers

clean:
	rm -f simplesh
