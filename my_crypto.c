#include "my_crypto.h"

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gcrypt.h>

struct _My_crypto {
	gcry_cipher_hd_t	_cipherhd;
	int		 	_cipher;
    //int				  _md;
};

My_crypto* new_My_crypto() {
#ifdef DEBUG
	fprintf(stderr, "in new_My_crypto\n"); //debug
#endif
	My_crypto* ret = malloc(sizeof(My_crypto));
	return ret;
}

int init_My_crypto(My_crypto* obj, const char alg[], int mode) {
	obj->_cipher = gcry_cipher_map_name(alg);
	//ret->_md = gcry_md_map_name("MD5");
	if (gcry_cipher_open(&(obj->_cipherhd), obj->_cipher, mode, 0) != GPG_ERR_NO_ERROR)
		return -1;
	return 0;
}


void delete_My_crypto(My_crypto* obj) {
#ifdef DEBUG
	fprintf(stderr, "in delete_My_crypto\n"); //debug
#endif
	gcry_cipher_close(obj->_cipherhd);
	obj->_cipherhd  = NULL;

	free(obj);
	obj = NULL;
}

int real_crypto(My_crypto* obj, const unsigned char key[], const unsigned key_len, const unsigned char input[], const unsigned in_len, unsigned char output[], int choose) //choose == 1 enc, == 2 dec
{
	int cipher = obj->_cipher;
	//fprintf(stderr, "cipher=%i\n", cipher);
	guint* real_keylen = g_new0(guint, 1);
	gcry_cipher_algo_info(cipher, GCRYCTL_GET_KEYLEN, NULL, real_keylen);
	//fprintf(stderr, "real_keylen=%i\n", *real_keylen);
	
	gchar* real_key = g_new0(gchar, *real_keylen);
	int i;
	for (i = 0; i < *real_keylen; i++)
		real_key[i] = key[i % key_len];

	if (gcry_cipher_setkey(obj->_cipherhd, real_key, *real_keylen) != GPG_ERR_NO_ERROR) {
		gcry_cipher_close(obj->_cipherhd);
		obj->_cipherhd  = NULL;
	}

	unsigned int blocksize;
	gcry_cipher_algo_info(obj->_cipher, GCRYCTL_GET_BLKLEN, NULL, &blocksize);

	//guchar* salts = g_malloc0( 1 * blocksize);
	//gcry_cipher_setiv(cipherhd, salts, blocksize);
	//gcry_cipher_encrypt(cipherhd, salts, 1 * blocksize, NULL, 0);

	//fprintf(stderr, "salts = %s\n", salts);
	gchar* iv = g_new0(gchar, blocksize);
	memset(iv, 0, blocksize);
	gcry_cipher_setiv(obj->_cipherhd, iv, blocksize);

	unsigned out_len = in_len;
	//fprintf(stderr, "in_len=%i\n", in_len);
	//fprintf(stderr, "choose=%i\n", choose);

	if(choose==1)
		gcry_cipher_encrypt(obj->_cipherhd, output, out_len, input, in_len);
	else if(choose==2)
		gcry_cipher_decrypt(obj->_cipherhd, output, out_len, input, in_len);

	*real_keylen = 0;
	g_free(real_keylen);
	memset(real_key, 0, *real_keylen);
	g_free(real_key);
	memset(iv, 0, blocksize);
	g_free(iv);
	return 0;
}

int my_enc(My_crypto* obj, const unsigned char key[], const unsigned key_len, const unsigned char input[], const unsigned in_len, unsigned char output[])
{
	return real_crypto(obj, key, key_len, input, in_len, output, 1); //choose == 1 enc, == 2 dec
}

int my_dec(My_crypto* obj, const unsigned char key[], const unsigned key_len, const unsigned char input[], const unsigned in_len, unsigned char output[])
{
	return real_crypto(obj, key, key_len, input, in_len, output, 2); //choose == 1 enc, == 2 dec
}

int my_hash(const char alg[], const unsigned char input[], const unsigned int input_len, unsigned char output[]) {
	int md = gcry_md_map_name(alg);
	unsigned int mdlen = gcry_md_get_algo_dlen(md);
	gchar *keybuf = g_new0(gchar, mdlen);
	memset(keybuf, 0, mdlen);
	gcry_md_hash_buffer(md, keybuf, input, input_len);
	
	//copy
	size_t i = 0;
	for(i = 0; i<mdlen; ++i) {
		output[i] = keybuf[ i%mdlen ];
	}
	return 0;
}

