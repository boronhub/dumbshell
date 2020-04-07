shell: myshell run.c
	gcc run.c -o myshell

myshell: shell.c
	gcc shell.c -o myshell

clean:
	rm Cshell myshell