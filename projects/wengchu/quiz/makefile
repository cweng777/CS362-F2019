default: testme

testme:
	gcc -o testme testme.c -Wall -fpic -coverage -lm -std=c99
	./testme

clean:
	rm testme *.gcda *.gcno *.gcov
