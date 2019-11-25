CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


# ------------------------------------------------
# For group project: unit tests
unittest1: unittest1.c dominion.o rngs.o
	gcc  -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc  -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc  -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc  -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
  
unittest9: unittest9.c dominion.o rngs.o
	gcc  -o unittest9 -g  unittest9.c dominion.o rngs.o $(CFLAGS)

unittest10: unittest10.c dominion.o rngs.o
	gcc  -o unittest10 -g  unittest10.c dominion.o rngs.o $(CFLAGS)

unittest11: unittest11.c dominion.o rngs.o
	gcc  -o unittest11 -g  unittest11.c dominion.o rngs.o $(CFLAGS)

unittestresults: unittest9 unittest10 unittest11
	@echo "--- UNIT TEST RESULTS ---" > unittestresults.out
  ./unittest1 >> unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out
	# ./unittest5
	# ./unittest6
	# ./unittest7
	# ./unittest8
	./unittest9 >> unittestresults.out
	./unittest10 >> unittestresults.out
	./unittest11 >> unittestresults.out
	@echo "--- COVERAGE ---" >> unittestresults.out
	gcov dominion.c -bf >> unittestresults.out

# ------------------------------------------------

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe \
	unittest1 unittest2 unittest3 unittest4 unittest5 unittest6 unittest7 unittest8 unittest9 unittest10 unittest11 \
	unittestresults.out
