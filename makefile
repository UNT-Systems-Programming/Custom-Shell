CC=gcc
CFLAGS=-Wall -g
shell: shell.c
	$(CC) -o shell $(CFLAGS) shell.c
clean:
	$(RM) shell
run:
	./shell