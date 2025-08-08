all:
	gcc -g -Wall -Wextra src/main.c src/win_impl.c -o bin/main

clear:
	@rm bin/**