
all: encrypt.c decrypt.c tsbox_aes.o file_handling.o
	gcc -g encrypt.c tsbox_aes.o file_handling.o -o encrypt
	gcc -g decrypt.c tsbox_aes.o file_handling.o -o decrypt
tsbox_aes: tsbox_aes.c
	gcc -c tsbox_aes.c

file_handling: file_handling.c
	gcc -c file_handling.c

teste: all
	./out in1 0123456789abcdef 
