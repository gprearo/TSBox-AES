#include "file_handling.h"

char *get_input_string(char *file_name, unsigned long *str_size) {
	FILE *input = fopen(file_name, "r") ;

	if (!input) {
		return NULL ;
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

	*str_size = vector_size + 1 ;
	return input_array ;
}


t_state *get_state_arrays(char *input_array, int vector_size, int *num_states) {
	//Ignora o '\0'
	vector_size-- ;	

	//Cria uma variavel local com o numero de states e atribui o valor ao paremetro por referência
	int n_states = *num_states = vector_size / STATE_SIZE ;

	//Cria um vetor de states
	t_state *states = (t_state *) malloc(sizeof(t_state) * n_states) ;

	//Inicializa cada t_state do vetor states com um ponteiro para um vetor que contém
	//os endereços das colunas daquele state
	int i ;
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

