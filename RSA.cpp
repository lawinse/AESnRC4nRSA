#include "RSA.h"
#include <assert.h>

RSA::RSA(){
	generate_two_big_primes(p,q);
	phi = (p-1)*(q-1);
	n = p*q;
	ULL y;
	while(true) {
		e = ran()%(phi-3)+3;
		if (phi%e==0) continue;
		ULL gcd = exgcd(e,phi,d,y);
		if (gcd == 1ULL && d > 0 && d < n) break;
	}

}

ULL mod_pro(ULL x,ULL y,ULL n) { 
	ULL ret = 0,tmp = x % n; 
	while(y) { 
		if (y & 0x1)
			if((ret += tmp) > n) ret -= n; 
		if ((tmp<<=1)>n) tmp -= n; 
		y>>=1; 
	} 
	return ret; 
}

ULL mod(ULL a,ULL b,ULL c) { 
	ULL ret = 1; 
	while(b) { 
		if (b & 0x1) ret = mod_pro(ret,a,c); 
		a = mod_pro(a,a,c); 
		b >>= 1; 
	} 
	return ret; 
}

ULL RSA::ran() { 
	ULL ret=rand(); 
	return (ret<<31)+rand(); 
} 

bool RSA::is_prime(ULL n,int t) { 
	if(n < 2) return false; 
	if(n == 2) return true; 
	if(n%2==0) return false; 
	ULL k=0,m,a,i; 
	for(m = n-1;!(m & 1);m >>= 1,++k); 
	while(t--) { 
		a = mod(ran()%(n-2)+2,m,n); 
		if(a != 1) { 
			for(i = 0;i < k && a!=n-1; ++i) 
				a = mod_pro(a,a,n); 
			if(i >= k) return false; 
		} 
	} 
	return true; 
}

int RSA::enum_prime_less_than(int n, UI *p) {
	if (n<=2) return 0;
	bool *notPrime = new bool [n+1];
	memset(notPrime, 0, sizeof(bool)*(n+1));
	int cnt = 0;
	p[0] = 1;
	int tmp;
	for (int i=2; i<n; ++i) {
		if (!notPrime[i]) p[++cnt] = i;
		for (int j=1; j<=cnt; ++j) {
			if ((tmp = p[j]*i) >= n) break;
			notPrime[tmp] = true;
			if (i%p[j] == 0) break;
			}
	}
	delete [] notPrime;
	return cnt;
}

/* http://bindog.github.io/blog/2014/07/19/how-to-generate-big-primes */
void RSA::generate_two_big_primes(ULL &a, ULL &b) {
	// 9-bits intergers
	a = 1e8+ran()%(ULL(9e8));
	if (a%2==0) ++a;
	b = 1e8+ran()%(ULL(9e8));
	if (b%2==0) ++b;
	static UI* primes_less_than_1e4 = new UI[int(1e4+1)];
	int cnt = enum_prime_less_than(int(1e4), primes_less_than_1e4);
	int i;
	while(true) {
		bool f = false;
		for (i=3; a>primes_less_than_1e4[i] && i<cnt; ++i) {
			if (a%primes_less_than_1e4[i]==0) {f=true;break;}
		}
		if (f) {a+=2;continue;}
		if (!is_prime(a,10)) a+=2;
		else break;
	}

	while(true) {
		if (a==b) {b+=2;continue;}
		bool f = false;
		for (i=3; b>primes_less_than_1e4[i] && i<cnt; ++i) {
			if (b%primes_less_than_1e4[i]==0) {f=true;break;}
		}
		if (f) {b+=2;continue;}
		if (!is_prime(b,10)) b+=2;
		else break;
	}
}

ULL RSA::exgcd(ULL a, ULL b, ULL& x, ULL& y) {
	if(b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	ULL gcd = exgcd(b, a%b, x, y);
	ULL t = y;
	y = x-(a/b)*(y);
	x = t;
	return gcd;
}

void RSA::cipher(ULL *in, size_t len, ULL *out, ULL _e, ULL _n) {
	for (int i=0; i<len; ++i) { 
		assert(in[i] < _n);
		out[i] = mod(in[i],_e,_n);
	}
}

void RSA::decipher(ULL *in, size_t len, ULL *out, ULL _d, ULL _p, ULL _q) {
	ULL N = _p*_q;
	for (int i=0; i<len; ++i) 
		out[i] = mod(in[i],_d,N);
}

void RSA::demoRSA() {
	printf("Demoing RSA ... \n");
	srand(time(0));
	RSA rsa;
	rsa.print_key();
	int txtlen = 10;
	ULL *in = new ULL[txtlen], *out = new ULL[txtlen], *din = new ULL[txtlen];
	for (int i=0; i<txtlen; ++i) {in[i]=(ULL)rand();}
	printf("original msg:\n");
	int i;
	for (i = 0; i < txtlen; ++i) {
		printf("%x ", (UI)in[i]);
	}
	printf("\n");
	ULL _e,_n,_d,_p,_q;
	rsa.get_public_key(_e,_n);
	RSA::cipher(in,txtlen,out,_e,_n);
	printf("cipher msg:\n");
	for (i = 0; i < txtlen; ++i) {
		printf("%x ", (UI)out[i]);
	}
	printf("\n");
	rsa.get_private_key(_d,_p,_q);
	RSA::decipher(out,txtlen,din,_d,_p,_q);
	printf("decipher msg:\n");
	for (i = 0; i < txtlen; ++i) {
		printf("%x ", (UI)din[i]);
	}
	printf("\n");

}

void RSA::testRSA(size_t txtlen, int repeat_time) {
	assert(txtlen>0);
	srand(time(0));
	printf("Testing RSA with txtlen %lu ...\n",txtlen);

	ULL *in = new ULL[txtlen];
	ULL *out = new ULL[txtlen];
	ULL *din = new ULL[txtlen];
	
	for (int t=0; t<repeat_time; ++t) {
		RSA rsa;
		for (int i=0; i<txtlen; ++i) {in[i]=(ULL)rand();}
		ULL _e,_n,_d,_p,_q;
		rsa.get_public_key(_e,_n);
		RSA::cipher(in,txtlen,out,_e,_n);
		rsa.get_private_key(_d,_p,_q);
		RSA::decipher(out,txtlen,din,_d,_p,_q);
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
	delete [] in;
	delete [] out;
	delete [] din;

}