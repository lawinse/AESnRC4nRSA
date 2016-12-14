#include "RC4.h"
#include <assert.h>

void RC4::set_for_cipher(int kl=8,uint8_t *_key=NULL) {
	keylen = kl;
	if (_key == NULL) {
		srand(time(0));
		K.push_back(rand()%MAX_VALUE);
	} else {
		for (int i=0; i<kl; ++i) K.push_back(_key[i]);
	}
}

void RC4::cipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks) {
	int i,j,t;
	for(i=0;i<256;++i){
		S[i]=i;
		T[i]=K[i%keylen];
	}

	j=0;
	for (i=0; i<256; ++i) {
		j=(j+S[i]+T[i])%256;
		std::swap(S[i],S[j]);
	}

	for (int k=0; k<len; ++k) {
		i=(i+1)%256;
		j=(j+S[i])%256;
		std::swap(S[i],S[j]);
		t=(S[i]+S[j])%256;
		ks[k] = S[t]; 
		out[k] = ks[k]^in[k];
	}
}

void RC4::decipher(uint8_t *in, size_t len, uint8_t *out, uint8_t *ks) {
	for (int k=0; k<len; ++k) {
		out[k] = ks[k]^in[k];
	}
}

void RC4::testRC4(size_t keylen, size_t txtlen, int repeat_time) {
	assert(keylen > 4 || txtlen >= keylen);
	printf("Testing RC4 with keylen %lu and txtlen %lu ...\n",keylen,txtlen);
	fflush(stdout);
	srand(time(0));

	uint8_t *in = new uint8_t[txtlen];
	uint8_t *out = new uint8_t[txtlen];
	uint8_t *ks = new uint8_t[txtlen];
	uint8_t *din = new uint8_t[txtlen];

	RC4 rc4;

	for (int t=0; t<repeat_time; ++t) {
		for (int i=0; i<txtlen; ++i) in[i] = rand()%MAX_VALUE;
		rc4.set_for_cipher(keylen);
		rc4.cipher(in,txtlen,out,ks);
		rc4.set_for_decipher();
		rc4.decipher(out,txtlen,din,ks);
		for (int i=0; i<txtlen; ++i) {
			if (in[i] != din[i]) {
				printf("[Test Result] Failed !!!\n");
				goto TEST_FIN;
			}
		}
	}
	printf("[Test Result] Passed all %d tests !!!\n",repeat_time );
	
	TEST_FIN:
	fflush(stdout);
	delete [] ks;
	delete [] in;
	delete [] out;
	delete [] din;
}

void RC4::demoRC4() {
	printf("Demoing RC4 ... \n");
	RC4 rc4;
	uint8_t key[] = {
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
		0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1a, 0x1b,
		0x1c, 0x1d, 0x1e, 0x1f
	};

	rc4.set_for_cipher(sizeof(key),key);

	uint8_t in[] = {
		0x00, 0x11, 0x22, 0x33,
		0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb,
		0x88, 0x99, 0xaa, 0xbb,
		0xcc, 0xdd, 0xee, 0xff,
		0xcc, 0xdd, 0xee, 0xff,
		0x88, 0x99, 0xaa, 0xbb,
		0xcc, 0xdd, 0xee, 0xff
	};
	uint8_t *out = new uint8_t[sizeof(in)];
	uint8_t *ks = new uint8_t[sizeof(in)];
	uint8_t *din = new uint8_t[sizeof(in)];
	rc4.cipher(in,sizeof(in),out,ks);
	int i;
	printf("original msg:\n");
	for (i = 0; i < sizeof(in); ++i) {
		printf("%x ", in[i]);
	}
	printf("\n");
	printf("key stream:\n");
	for (i = 0; i < sizeof(in); ++i) {
		printf("%x ", ks[i]);
	}
	printf("\n");
	printf("cipher msg:\n");
	for (i = 0; i < sizeof(in); ++i) {
		printf("%x ", out[i]);
	}
	printf("\n");

	rc4.set_for_decipher();
	rc4.decipher(out,sizeof(in),din,ks);
	printf("decipher msg:\n");
	for (i = 0; i < sizeof(in); ++i) {
		printf("%x ", din[i]);
	}
	printf("\n");
}



