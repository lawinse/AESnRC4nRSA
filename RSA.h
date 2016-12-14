#ifndef RSA_H
#define RSA_H
typedef unsigned long long ULL; 
typedef unsigned int UI;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

class RSA{
public:
	RSA();
	~RSA(){};
	void print_key() {
		printf("RSA keys: ");
		printf("p=%llu, q=%llu, phi=%llu, e=%llu, d=%llu", p,q,phi,e,d);
		printf("\n");
	}
	void get_public_key(ULL &_e, ULL &_n) {_e=e,_n=n;}
	void get_private_key(ULL &_d, ULL &_p, ULL &_q) {_p=p,_q=q,_d=d;}

	static void cipher(ULL *in, size_t len, ULL *out, ULL _e, ULL _n);
	static void decipher(ULL *in, size_t len, ULL *out, ULL _d, ULL _p, ULL _q);
	static void testRSA(size_t txtlen, int repeat_time=100);
	static void demoRSA();
private:
	ULL p,q,phi,e,d,n;
	ULL ran();
	bool is_prime(ULL n,int t);
	int enum_prime_less_than(int n, UI *p);
	void generate_two_big_primes(ULL &a, ULL &b);
	ULL exgcd(ULL a, ULL b, ULL& x, ULL& y);

};

#endif