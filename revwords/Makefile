C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib

all: clean revwords

revwords:
	gcc $(C_FLAGS) -o revwords revwords.c -lhelpers

clean:
	rm -f revwords
