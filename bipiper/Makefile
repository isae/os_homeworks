C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib

all: bipiper

bipiper: forking.c polling.c
	gcc $(C_FLAGS) -o forking forking.c -lbufio -lhelpers
	gcc $(C_FLAGS) -o polling polling.c -lbufio -lhelpers

clean:
	rm -f forking
	rm -f polling
