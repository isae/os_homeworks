C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib
FILES= revwords.c

all: revwords

revwords:
	gcc $(C_FLAGS) -o revwords $(FILES) -lhelpers

clean:
	rm -f revwords
