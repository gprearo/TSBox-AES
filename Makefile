
all: tsbox-aes.c file_handling.o
	gcc -g tsbox-aes.c file_handling.o -o out 

file_handling: file_handling.c
	gcc -c file_handling.c

teste: all
	./out in1
