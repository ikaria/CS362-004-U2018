CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

utils.o: utils.h utils.c
	gcc -c utils.c -lpthread $(CFLAGS)

unitTest1: dominion.o rngs.o utils.o unittest1.c 
	gcc -o u1 unittest1.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

unitTest2: dominion.o rngs.o utils.o unittest2.c 
	gcc -o u2 unittest2.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

unitTest3: dominion.o rngs.o utils.o unittest3.c 
	gcc -o u3 unittest3.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

unitTest4: dominion.o rngs.o utils.o unittest4.c 
	gcc -o u4 unittest4.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

cardTest1: dominion.o rngs.o utils.o cardtest1.c 
	gcc -o c1 cardtest1.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

cardTest2: dominion.o rngs.o utils.o cardtest2.c 
	gcc -o c2 cardtest2.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

cardTest3: dominion.o rngs.o utils.o cardtest3.c 
	gcc -o c3 cardtest3.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

cardTest4: dominion.o rngs.o utils.o cardtest4.c 
	gcc -o c4 cardtest4.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

rand1: dominion.o rngs.o utils.o randomtestadventurer.c 
	gcc -o r1 randomtestadventurer.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

rand2: dominion.o rngs.o utils.o randomtest2.c 
	gcc -o r2 randomtest2.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

unittestresults.out: dominion.o utils.o
	gcc -o u1 unittest1.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o u2 unittest2.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o u3 unittest3.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o u4 unittest4.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o c1 cardtest1.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o c2 cardtest2.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o c3 cardtest3.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)
	gcc -o c4 cardtest4.c -g dominion.o utils.o rngs.o -lpthread $(CFLAGS)

	./u1 >> unittestresults.out
	./u2 >> unittestresults.out
	./u3 >> unittestresults.out
	./u4 >> unittestresults.out
	./c1 >> unittestresults.out
	./c2 >> unittestresults.out
	./c3 >> unittestresults.out
	./c4 >> unittestresults.out

	gcov -f -b dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
