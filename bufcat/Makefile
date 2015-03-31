C_FLAGS= -L../lib -Wl,-rpath=../lib -Wall -fPIC -I../lib
DEBUG_FLAGS= 
FILES= bufcat.c

all: bufcat

bufcat: $(FILES)
	gcc $(DEBUG_FLAGS) $(C_FLAGS) -o bufcat bufcat.c -lbufio

clean:
	rm -f bufcat
