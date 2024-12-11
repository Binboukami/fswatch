all:
	gcc -g -Wall -Wextra src/main.c -o bin/main

clean:
	rm bin/**