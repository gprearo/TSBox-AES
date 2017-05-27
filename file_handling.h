#include <stdio.h>
#include <stdlib.h>
#include "state.h"

/*
	Função: get_input_string

	Parâmetros:
		file_name: nome do arquivo a ser lido
		str_size: endereço do unsigned long que será colocad oo tamanho da entrada (com '\0')

	Descrição:
		Lê o arquivo de entrada e armazena em um vetor de char com o tamanho
		certo (multiplo do tamanho do state)

*/
char *get_input_string(char *file_name, unsigned long *input_size, unsigned long *output_size) ;

char *get_input_byte(char *file_name, unsigned long *input_size) ;

/*
	Função: get_state_arrays

	Parâmetros:
		input_array: string com o texto de entrada já no tamanho certo (multiplo do tamanho do state)
		vector_size: tamanho do texto de entrada (contando o '\0')
		num_states: endereço do int onde será colocada a quantidade de states gerada

	Descrição:
		Gera um vetor de states com base no texto de entrada
		ATENÇÃO: para acessar o valor de um byte de um states fazer:
			state[i][col][row]
				i - posição do state
				col - coluna do state
				row - linha do state
*/
t_state *get_state_arrays(char *input_array, int vector_size, int *num_states) ;

void write_output(char *file_name, char *string, int size) ;

void write_output_text(char *file_name, char *string) ;
