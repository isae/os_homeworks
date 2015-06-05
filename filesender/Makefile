C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -std=c99 -g -I../lib
FILES= filesender.c

all: filesender

filesender: $(FILES)
	gcc $(C_FLAGS) -o filesender $(FILES) -lbufio -lhelpers

clean:
	rm -f filesender
