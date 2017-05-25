
all: tsbox-aes.c
	gcc -g tsbox-aes.c -o out 


teste: all
	./out in1
