#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
#include "tsbox_aes.h"
#include "tsbox.h"

int main(int argc, char *argv[]) {

        if (argc < 3) {
                printf("usage: %s filename key\n"
                       "\tfilename: file with plain-text that will be encrypted\n"
                       "\tkey: 128-bit key used to encrypt\n\n", argv[0]) ;
                return 3 ;
        }

        unsigned long input_size ;
        unsigned long output_size ;
        char *input_str = get_input_string(argv[1], &input_size, &output_size) ;
		char *key = argv[2] ;
        if (!input_str){
                printf("Invalid file!!\n") ;
                return 1 ;
        }

        if(strlen(argv[2]) != Nk*4) {
                printf("Invalid key!!\n") ;
                return 2 ;
        }

        int num_states ;
        t_state *states = get_state_arrays(input_str, output_size, &num_states) ;

        char **expanded_key = key_expansion(key) ;

        int i, j ;
        for (i = 0; i < num_states; i++) {
                add_round_key(states[i], expanded_key, 0) ;

				col_transpose_sbox(key);
                for (j = 1; j < Nr; j++) {
						t_sub_bytes(states[i]) ;
                        shift_rows(states[i]) ;
                        mix_columns(states[i]) ;
                        add_round_key(states[i], expanded_key, j) ;
                }

				t_sub_bytes(states[i]) ;
                shift_rows(states[i]) ;
                add_round_key(states[i], expanded_key, Nr) ;
        }
        write_output("output_cipher.txt", input_str, output_size) ;
        printf("Input size: %lu\n", input_size);
        printf("Output size: %lu\n", output_size);

        return 0 ;
}
