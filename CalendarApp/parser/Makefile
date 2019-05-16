CFLAGS = -Wall -pedantic -Iinclude -std=c11 -fpic

all: list parser

list: src/LinkedListAPI.c
	gcc $(CFLAGS) -shared ./src/LinkedListAPI.c -o ./bin/liblist.so

parser: src/CalendarParser.c src/helper.c ./src/LinkedListAPI.c
	gcc $(CFLAGS) -shared ./src/CalendarParser.c ./src/helper.c ./src/LinkedListAPI.c -o ../libcal.so

clean:
	rm ./bin/*.so
