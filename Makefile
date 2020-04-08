shell: myshell shell.c
	gcc shell.c -o myshell

myshell: shell.c
	gcc shell.c -o myshell

clean:
	rm Cshell myshell