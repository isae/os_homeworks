C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -I../lib

all: clean cat

cat:
	gcc $(C_FLAGS) -o cat cat.c -lhelpers

clean:
	rm -f cat
