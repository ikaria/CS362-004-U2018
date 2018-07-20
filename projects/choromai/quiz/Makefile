CFLAGS = -Wall -fpic -coverage -lm -std=c99

test: testme.c 
	gcc -c testme.c -g  $(CFLAGS)

all: test 

clean:
	rm -f *.o *.gcov *.gcda *.gcno *.so *.out test test.exe
