#ifndef MY_CRYPTO_H
#define MY_CRYPTO_H 1

#include <gcrypt.h>

typedef struct _My_crypto	My_crypto;
/*
typedef enum _Cipher_mode { 
	CBC, OFB, CFB 
} Cipher_mode;
*/
My_crypto* new_My_crypto();
int init_My_crypto(My_crypto* obj, const char alg[], int mode);
void delete_My_crypto(My_crypto* obj);

int my_enc(My_crypto* obj, const unsigned char key[], const unsigned key_len, const unsigned char input[], const unsigned in_len, unsigned char output[]);
int my_dec(My_crypto* obj, const unsigned char key[], const unsigned key_len, const unsigned char input[], const unsigned in_len, unsigned char output[]);

int my_hash(const char alg[], const unsigned char input[], const unsigned int input_len, unsigned char output[]);

#endif
