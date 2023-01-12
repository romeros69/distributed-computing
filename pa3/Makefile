run:
	clang -std=c99 -Wall -pedantic -L. *.c -lruntime && ./a.out -p 5 10 20 30 40 50
	# clang -std=c99 -Wall -fsanitize=address,undefined,leak -pedantic -g -L. *.c -lruntime
clean:
	rm a.out