#ifndef RC4_H
#define RC4_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <time.h>
#include <string.h>

#define BITS_PER_BYTES 8
#define MAX_VALUE (1<<(BITS_PER_BYTES))

class RC4{
public:
	RC4(){};
	~RC4(){}
	void set_for_cipher(int kl, uint8_t *_key);
	void set_for_decipher(){K.clear();memset(S,0,sizeof(S)),memset(T,0,sizeof(T));}
	void cipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks);
	void decipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks);

	static void testRC4(size_t keylen, size_t txtlen, int repeat_time=1000);
	static void demoRC4();
private:
	int keylen;
	std::vector<uint8_t> K;
	uint8_t S[256];
	uint8_t T[256];
};

#endif