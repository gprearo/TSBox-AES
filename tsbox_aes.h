#ifndef TSBOX_AES_H
#define TSBOX_AES_H


//Substitui o byte do state pelo equivalente da sbox
void sub_bytes(t_state s) ;
void inv_sub_bytes(t_state s) ;

//Faz shift de um em uma linha do state
void shift_row(t_state s, int row) ;
//Faz a operação ShiftRows do algoritmo
//Linha 0 - 0 shifts
//Linha 1 - 1 shift
//Linha 2 - 2 shifts
//Linha 3-  3 shifts
void shift_rows(t_state s) ;
void inv_shift_rows(t_state s) ;

//Aplica a função MixColumn em uma coluna
void mix_column(char *column) ;
//Aplica a função MixColumns em um state
void mix_columns(t_state s) ;
void inv_mix_columns(t_state s) ;


void word2hexa(char *w) ;
void rot_word(char *word) ;

void cpy_word(char *to, char *from) ;
void sub_word(char *w) ;


//Expande a chave para gerar os valores de chave de cada round
char **key_expansion(char *key) ;


//realiza XOR entre duas colunas
void xor_word(char *res, char* a, char *b) ;


//Faz xor do state com a chave do round
void add_round_key(t_state s, char **w, int round) ;
void inv_add_round_key(t_state s, char **w, int round) ;


void t_sub_bytes(t_state s) ;
void t_inv_sub_bytes(t_state s) ;
#endif // TSBOX_AES_H
