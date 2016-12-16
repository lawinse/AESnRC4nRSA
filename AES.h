#ifndef AES_H
#define AES_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BITS_PER_BYTES 8
#define MAX_VALUE (1<<(BITS_PER_BYTES))

/* AES 128,192,256 */
class AES {
public:
	AES(uint8_t *key, size_t n, int type);
	~AES();
	void cipher(uint8_t *in, uint8_t *out);
	void decipher(uint8_t *in, uint8_t *out);
	void show_kep();

	static void testAES(size_t aestype=128, size_t keylen=192, int repeat_time=1000);
	static void demoAES();

private:
	static uint8_t sbox[256];
	static uint8_t inv_sbox[256];
	uint8_t *kep;

	int Nb;
	int Nk, Nr;
	int AES_TYPE;
	uint8_t gf_mul(uint8_t a, uint8_t b);
	uint8_t gf_add(uint8_t a, uint8_t b);
	uint8_t gf_sub(uint8_t a, uint8_t b);
	void gf_mulword(uint8_t *a, uint8_t *b, uint8_t *ret);
	void sub_word(uint8_t *word);
	void sub_byte(uint8_t &byte, uint8_t *box);
	void lrotate_word(uint8_t *word);
	uint8_t round_con(int i);

	void key_expansion(uint8_t *key);
	void add_round_key(uint8_t *state, int rnd);
	void sub_bytes(uint8_t *state){_sub_bytes(state,sbox);}
	void mix_columns(uint8_t *state){_mix_columns(state,false);}
	void shift_rows(uint8_t *state){_shift_rows(state,false);}
	void inv_sub_bytes(uint8_t *state){_sub_bytes(state,inv_sbox);}
	void inv_mix_columns(uint8_t *state){_mix_columns(state,true);}
	void inv_shift_rows(uint8_t *state){_shift_rows(state,true);}

	void _sub_bytes(uint8_t *state, uint8_t *box);
	void _mix_columns(uint8_t *state, bool isinv);
	void _shift_rows(uint8_t *state, bool isinv);
};


#endif
