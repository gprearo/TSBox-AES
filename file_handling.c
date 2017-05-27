#include "file_handling.h"

char *get_input_string(char *file_name, unsigned long *input_size, unsigned long *output_size) {
	FILE *input = fopen(file_name, "r") ;

	if (!input) {
		return NULL ;
	}

	fseek(input, 0L, SEEK_END) ;
	unsigned long file_size = ftell(input);
        *input_size = file_size;

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

        *output_size = vector_size ;
	return input_array ;
}

char *get_input_byte(char *file_name, unsigned long *input_size) {
        FILE *input = fopen(file_name, "r") ;

        if (!input) {
                return NULL ;
        }

        fseek(input, 0L, SEEK_END) ;
        unsigned long file_size = ftell(input);
        *input_size = file_size;

        char *input_array = (char *) malloc(sizeof(char)*(file_size)) ;

        fseek(input, 0L, SEEK_SET) ;

        fread(input_array, file_size, sizeof(char), input) ;
        fclose(input) ;

        return input_array ;
}

t_state *get_state_arrays(char *input_array, int vector_size, int *num_states) {
	//Ignora o '\0'
//	vector_size-- ;

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

void write_output(char *file_name, char *string, int size) {
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(string, sizeof(char), size, f);
    fclose(f);
    return;
}
void write_output_text(char *file_name, char *string) {
    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, "%s", string);
    fclose(f);
    return;
}

