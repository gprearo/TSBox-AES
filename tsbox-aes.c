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
#include "file_handling.h"
#include "state.h"




static const char sbox[256] =   {
	//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const unsigned char rcon[256] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
} ;

//Substitui o byte do state pelo equivalente da sbox
void sub_bytes(t_state s) {
	int i, j ;
	for (i = 0; i < Nb; i++) {
		for (j  = 0; j < 4; j++) {
			s[i][j] = sbox[0xff & s[i][j]] ;
		}
	}
}

//Faz shift de um em uma linha do state
void shift_row(t_state s, int row) {
	char tmp = s[0][row] ;
	int i ;
	for (i = 0; i < Nb - 1; i++) {
		s[i][row] = s[i + 1][row] ;
	}
	s[Nb - 1][row] = tmp ;
}

//Faz a operação ShiftRows do algoritmo
//Linha 0 - 0 shifts
//Linha 1 - 1 shift
//Linha 2 - 2 shifts
//Linha 3-  3 shifts
void shift_rows(t_state s) {
	int i ;
	for (i  = 0; i < 4; i++) {
		int j ;
		for (j = 0; j < i; j++) {
			shift_row(s, i) ;
		}
	}
}

//Aplica a função MixColumn em uma coluna
void mix_column(char *column) {
	char old[4] ;
	char oldx2[4] ;
	char high_bit ;

	int i ;

	/*
		Galoi é um latifundiário muito rico e poderoso que não é egoísta.
		Ele deixa as pessoas usarem seus campos, mas com duas condições:
			Toda soma dentro de seus campos tem que ser substituída por XOR
			Quando se multiplica um byte que tem bit mais significativo igual a 1 
				por 2, deve-se realizar um XOR por 0x1B 
	*/
	for (i = 0; i < 4; i++) {
		old[i] = column[i] ;

		high_bit = (column[i] & 0x80 ? 0xff : 0x00) ;

		oldx2[i] = column[i] << 1 ;
		oldx2[i] ^= 0x1b &  high_bit ;
	}

	column[0] = oldx2[0] ^ old[1] ^ oldx2[1] ^ old[2] ^ old[3] ;
	column[1] = old[0] ^ oldx2[1] ^ oldx2[2] ^ old[2] ^ old[3] ;
	column[2] = old[0] ^ old[1] ^ oldx2[2] ^ oldx2[3] ^ old[3] ;
	column[3] = oldx2[0] ^ old[0] ^ old[1] ^ old[2] ^ oldx2[3] ;

}

//Aplica a função MixColumns em um state
void mix_columns(t_state s) {
	int i ;
	for (i = 0; i < Nb; i++) {
		mix_column(s[i]) ;
	}
}


void word2hexa(char *w) {
	int i;
	for (i = 0; i < 4; i++) {
		printf("%02x", w[i] & 0xff) ;
	}
	printf("\n") ;
}


void rot_word(char *word) {
	char c = word[0] ;
	word[0] = word[1] ;
	word[1] = word[2] ;
	word[2] = word[3] ;
	word[3] = c ;
}

void cpy_word(char *to, char *from) {
	int i ;
	for (i = 0; i < 4; i++) {
		to[i] = from[i] ;
	}
}

void sub_word(char *w) {
	int i ;
	for (i = 0; i < 4; i++) {
		w[i] = sbox[w[i] & 0xff] ;
	}
}


//Expande a chave para gerar os valores de chave de cada round
char **key_expansion(char *key) {
	char tmp[4] ;

	//A função gera (Nb*(Nr+1)) words (4 bytes)
	char **w = (char **) malloc(sizeof(char *) * (Nb*(Nr+1))) ;

	//Coloca a chave original nas 4 primeiras colunas
	int i = 0 ;
	while (i < Nk) {
		w[i] = (char *) malloc(sizeof(char) * 4) ;
		int j ;
		for (j = 0; j < 4; j++) {
			w[i][j] = key[4*i+j] ;
		}
		i++ ;
	}

	//Gera as outras "chaves"
	i = Nk ;
	while (i < Nb * (Nr + 1)) {
		//Copia o valor da coluna anterior em uma coluna temporaria
		cpy_word(tmp, w[i-1]) ;

		//Se a coluna que está sendo processada é multipla de Nk
		if (i % Nk == 0) {
			 //Rotaciona a "word" anterior
			 rot_word(tmp) ;
			 //Substitui os valores com base na sbox
			 sub_word(tmp) ;
			 int j ;
			 //Faz o xor do primeiro byte com o valor correspondente na tabela Rcon
			 tmp[0] ^= rcon[i/Nk] ;
		}
		else if (Nk > 6 && i % Nk == 4) {
			//Considera o caso da chave ser maior que 128
			sub_word(tmp) ;
		}

		w[i] = (char *) malloc(sizeof(char) * 4) ;
		int j ;
		//Cada coluna vai ser igual ao XOR entre acoluna Nk posições
		//atrás e a word tmp
		for (j = 0; j < 4; j++) {
			w[i][j] = w[i-Nk][j] ^ tmp[j] ;
		}

		i++ ;
	}

	return w ;
}


//realiza XOR entre duas colunas
void xor_word(char *res, char* a, char *b) {
	int i ;
	for (i = 0; i < 4; i++) {
		res[i] = a[i] ^ b[i] ;
	}
}


//Faz xor do state com a chave do round
void add_round_key(t_state s, char **w, int round) {
	int i ;
	for (i = 0; i < Nb; i++) {
		xor_word(s[i], s[i], w[round*Nb+i]) ;
	}
}

int main(int argc, char *argv[]) {

	if (argc < 3) {
		printf("usage: ./%s filename key\n\n", argv[0]) ;
		return 2 ;
	}
	
	printf("Key length %d bits\n\n", KEY_LENGTH) ;

	unsigned long input_size ;
	char *input_str = get_input_string(argv[1], &input_size) ;

	if (!input_str){
		printf("Invalid file!!") ;
		return 1 ;
	}

	int num_states ;
	t_state *states = get_state_arrays(input_str, input_size, &num_states) ;
	
/*
	char input_str[] = {0x032, 0x043, 0x0f6, 0x0a8, 0x088, 0x05a, 0x030, 0x08d, 0x031, 0x031, 0x098, 0x0a2, 0x0e0, 0x037, 0x007, 0x034, '\0'} ;
	char key[] = {0x02b, 0x07e, 0x015, 0x016, 0x028, 0x0ae, 0x0d2, 0x0a6, 0x0ab, 0x0f7, 0x015, 0x088, 0x009, 0x0cf, 0x04f, 0x03c} ;
	int num_states ;
	t_state *states = get_state_arrays(input_str, 17, &num_states) ;
*/
	int i, j ;
	for (i = 0; i < 16; i++) {
		printf("%02x  ", (unsigned char) input_str[i]) ;
	}

	char **expanded_key = key_expansion(argv[2]) ;



	for (i = 0; i < num_states; i++) {
		add_round_key(states[i], expanded_key, 0) ;

		int k ;

		for (j = 1; j < Nr; j++) {
			sub_bytes(states[i]) ;
			shift_rows(states[i]) ;
			mix_columns(states[i]) ;
			add_round_key(states[i], expanded_key, j) ;
		}

		sub_bytes(states[i]) ;
		shift_rows(states[i]) ;
		add_round_key(states[i], expanded_key, Nr) ;
	}

	printf("\nout: ") ;
	for (i = 0; i < 16; i++) {
		printf("%02x  ", (unsigned char) input_str[i]) ;
	}
	printf("\nplain out: %s",input_str) ;
	
	return 0 ;
}
