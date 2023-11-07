default:
	gcc main.c functions.c -o shell
	./shell

compile:
	gcc main.c functions.c -o shell

debug:
	gcc -g main.c functions.c -o shell
	gdb ./shell