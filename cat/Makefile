C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -I../lib
FILES= cat.c

all: cat

cat: $(FILES)
	gcc $(C_FLAGS) -o cat cat.c -lhelpers

clean:
	rm -f cat
