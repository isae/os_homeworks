C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib

all: lenwords

lenwords:
	gcc $(C_FLAGS) -o lenwords lenwords.c -lhelpers

clean:
	rm lenwords
