typedef char** t_state ;
//Nb = Número de colunas do state
#define Nb 4

//Nk = Tamanho da chave (words de 32 bits)
#define Nk 4

//KEY_LENGTH = length of AES key in bits
#define KEY_LENGTH (Nk*32)

//STATE_SIZE = size of each block in bytes
#define STATE_SIZE (Nb*4)

