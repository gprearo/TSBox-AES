#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
#include "tsbox_aes.h"
#include "tsbox.h"

int main(int argc, char *argv[]) {

        if (argc < 3) {
                printf("usage: %s filename key\n"
                       "\tfilename: file with cipher-text that will be decrypted\n"
                       "\tkey: 128-bit key used to decrypt\n\n", argv[0]) ;
                return 3 ;
        }



        unsigned long output_size ;
        unsigned long input_size ;
        char *input_str = get_input_byte(argv[1], &input_size) ;
		char *key = argv[2] ;

        if (!input_str){
                printf("Invalid file!!\n") ;
                return 1 ;
        }

        if(strlen(argv[2]) != 16) {
                printf("Invalid key!!\n") ;
                return 2 ;
        }

        int num_states ;
        t_state *states = get_state_arrays(input_str, input_size, &num_states) ;

        char **expanded_key = key_expansion(key) ;

        int i, j ;
        for (i = 0; i < num_states; i++) {
                inv_add_round_key(states[i], expanded_key, Nr) ;

				col_transpose_sbox(key) ;
                for (j = Nr - 1; j > 0; j--) {
                        inv_shift_rows(states[i]);
						t_inv_sub_bytes(states[i]) ;
                        add_round_key(states[i], expanded_key, j) ;
                        inv_mix_columns(states[i]) ;
                }

                inv_shift_rows(states[i]) ;
				t_inv_sub_bytes(states[i]) ;
                add_round_key(states[i], expanded_key, 0) ;
        }
		output_size = strlen(input_str);
        write_output_text("output_plain.txt", input_str) ;
        printf("Input size: %lu\n", input_size);
        printf("Output size: %lu\n", output_size);

        return 0 ;
}
