
all: encrypt.c decrypt.c tsbox_aes.o file_handling.o tsbox.o
	gcc -g encrypt.c tsbox_aes.o file_handling.o tsbox.o -o encrypt
	gcc -g decrypt.c tsbox_aes.o file_handling.o tsbox.o -o decrypt
tsbox_aes: tsbox_aes.c
	gcc -c tsbox_aes.c

file_handling: file_handling.c
	gcc -c file_handling.c

tsbox: tsbox.c
	gcc -c tsbox.c

teste: all
	./out in1 0123456789abcdef 
