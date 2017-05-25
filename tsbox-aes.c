/*
	Authors: Guilherme Prearo & Guilherme Momesso
	Compiler: gcc (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609

	Description:
	TSBox-AES

	This is a project from Security Engineering class. The purpose is to implement a new encryption algorithm based on existing ones.

	How it should work

	We want to use AES encryption algorithm, but with a columnar transposition cipher on S-box, from AES algorithm, using AES key. This should make our encryption stronger, since the result will be more dependent of key.
*/


#include <stdio.h>
#include <stdlib.h>

//Nb = Block length divided by 32
//Nb = 4 -> 128 bits AES
#define Nb 4

//KEY_LENGTH = length of AES key in bits
#define KEY_LENGTH (Nb*32)

//STATE_SIZE = size of each block in bytes
#define STATE_SIZE (Nb*4)

//Cada state é uma matriz de char com Nb colunas e 4 linhas
typedef char** t_state ;

t_state *get_state_arrays(char *file_name, int *num_states) {
	FILE *input = fopen(file_name, "r") ;

	if (!input) {
		printf("Invalid file!\n\n") ;
		exit(1) ;
	}

	fseek(input, 0L, SEEK_END) ;
	unsigned long file_size = ftell(input);

	//O tamanho do nosso vetor tem que ser multiplo do tamanho do "State"
	unsigned long vector_size = file_size + (STATE_SIZE - (file_size % STATE_SIZE)) ;
	char *input_array = (char *) malloc(sizeof(char)*(vector_size + 1)) ;

	fseek(input, 0L, SEEK_SET) ; 

	fread(input_array, vector_size, sizeof(char), input) ;
	fclose(input) ;

	//Completa o vetor com zeros, caso necessário
	unsigned long i ;
	for (i = file_size + 1; i < vector_size; i++) {
		input_array[i] = 0 ;
	}


	//Cria uma variavel local com o numero de states e atribui o valor ao paremetro por referência
	int n_states = *num_states = vector_size / STATE_SIZE ;

	//Cria um vetor de states
	t_state *states = (t_state *) malloc(sizeof(t_state) * n_states) ;

	//Inicializa cada t_state do vetor states com um ponteiro para um vetor que contém
	//os endereços das colunas daquele state
	for (i = 0; i < n_states; i++) {
		states[i] = (t_state) malloc(sizeof(t_state) * Nb) ;
		int j ;
		for (j = 0; j < Nb; j++) {
			//Endereço inicial da coluna j do state i no vetor de entrada
			states[i][j] = input_array + i*STATE_SIZE + j*4 ;
		}
	}

	return states ;

}

int main(int argc, char *argv[]) {
	
	printf("Key length %d bits\n\n", KEY_LENGTH) ;

	int num_states ;
	t_state *states = get_state_arrays(argv[1], &num_states) ;

	int i, j, k ;
	for (i = 0; i < num_states; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < Nb; k++) {
				printf("%c\t", states[i][k][j]) ;
			}
			printf("\n") ;
		}
	}
	return 0 ;
}
