CC=gcc
.PHONY: clean

automaton: automaton.o error_handler.o
	$(CC) -o $@ $^ 

%.o: %.c
	$(CC) -c $^

error_tester: error_handler.o error_tester.o
	$(CC) -o $@ $^


clean:
	rm -f *.o automaton error_tester
