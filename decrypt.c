#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
#include "tsbox_aes.h"

int main(int argc, char *argv[]) {

        if (argc < 3) {
                printf("usage: %s filename key\n"
                       "\tfilename: file with cipher-text that will be decrypted\n"
                       "\tkey: 128-bit key used to decrypt\n\n", argv[0]) ;
                return 3 ;
        }

        unsigned long input_size ;
        char *input_str = get_input_string(argv[1], &input_size) ;

        if (!input_str){
                printf("Invalid file!!\n") ;
                return 1 ;
        }

        if(strlen(argv[2]) != 32) {
                printf("Invalid key!!\n") ;
                return 2 ;
        }

        int num_states ;
        t_state *states = get_state_arrays(input_str, input_size, &num_states) ;


//        char input_str[] = {0x032, 0x043, 0x0f6, 0x0a8, 0x088, 0x05a, 0x030, 0x08d, 0x031, 0x031, 0x098, 0x0a2, 0x0e0, 0x037, 0x007, 0x034, '\0'} ;
//        char key[] = {0x02b, 0x07e, 0x015, 0x016, 0x028, 0x0ae, 0x0d2, 0x0a6, 0x0ab, 0x0f7, 0x015, 0x088, 0x009, 0x0cf, 0x04f, 0x03c} ;
//        int num_states ;
//        t_state *states = get_state_arrays(input_str, 17, &num_states) ;

//        printf("plain-text hex: ");
//        int i, j ;
//        for (i = 0; i < 16; i++) {
//                printf("%02x  ", (unsigned char) input_str[i]) ;
//        }

//        char **expanded_key = key_expansion(argv[2]) ;
////        char **expanded_key = key_expansion(key) ;

//        for (i = 0; i < num_states; i++) {
//                add_round_key(states[i], expanded_key, 0) ;

//                int k ;

//                for (j = 1; j < Nr; j++) {
//                        sub_bytes(states[i]) ;
//                        shift_rows(states[i]) ;
//                        mix_columns(states[i]) ;
//                        add_round_key(states[i], expanded_key, j) ;
//                }

//                sub_bytes(states[i]) ;
//                shift_rows(states[i]) ;
//                add_round_key(states[i], expanded_key, Nr) ;
//        }

//        printf("\ncypher-text hex: ") ;
//        for (i = 0; i < 16; i++) {
//                printf("%02x  ", (unsigned char) input_str[i]) ;
//        }
//        printf("\ncypher-text string (between quotes): \"%s\"\n",input_str) ;

        return 0 ;
}
