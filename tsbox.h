
//Operação SubByte inversa, tomando como base a S-Box transposta
void t_inv_sub_byte(char *c) ;

//Operação SubByte coma S-Box transposta
void t_sub_byte(char *c) ;

//Operação SubByte
void sub_byte(char *c) ;

//Operação de transposição da S-Box
//	recebe uma chave de 16 bytes
void col_transpose_sbox(char *key) ;
