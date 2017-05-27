#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
#include "tsbox_aes.h"

int main(int argc, char *argv[]) {

        if (argc < 4) {
                printf("usage: %s filename size key\n"
                       "\tfilename: file with cipher-text that will be decrypted\n"
                       "\tsize: plain-text output size\n"
                       "\tkey: 128-bit key used to decrypt\n\n", argv[0]) ;
                return 3 ;
        }



        unsigned long output_size ;
        unsigned long input_size ;
        output_size = atoi(argv[2]);
        char *input_str = get_input_byte(argv[1], &input_size) ;
        char key[] = {0x02b, 0x07e, 0x015, 0x016, 0x028, 0x0ae, 0x0d2, 0x0a6, 0x0ab, 0x0f7, 0x015, 0x088, 0x009, 0x0cf, 0x04f, 0x03c} ;

        printf("%s\n", input_str);

        if (!input_str){
                printf("Invalid file!!\n") ;
                return 1 ;
        }

//        if(strlen(argv[2]) != 32) {
//                printf("Invalid key!!\n") ;
//                return 2 ;
//        }

        int num_states ;
        t_state *states = get_state_arrays(input_str, input_size, &num_states) ;

//        char **expanded_key = key_expansion(argv[2]) ;
        char **expanded_key = key_expansion(key) ;

        int i, j ;
        for (i = 0; i < num_states; i++) {
                inv_add_round_key(states[i], expanded_key, Nr) ;

                for (j = Nr - 1; j > 0; j--) {
                        inv_shift_rows(states[i]);
                        inv_sub_bytes(states[i]) ;
                        add_round_key(states[i], expanded_key, j) ;
                        inv_mix_columns(states[i]) ;
                }

                inv_shift_rows(states[i]) ;
                inv_sub_bytes(states[i]) ;
                add_round_key(states[i], expanded_key, 0) ;
        }

        write_output_text("output_plain.txt", input_str) ;
        printf("Input size: %d\n", input_size);
        printf("Output size: %d\n", output_size);

        return 0 ;
}
