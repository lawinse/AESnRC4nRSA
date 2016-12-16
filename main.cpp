#include "RC4.h"
#include "RSA.h"
#include "AES.h"

int main(int argc, char const *argv[])
{
	for (int type=128;type<=256;type+=64)
		for (int klen=128;klen<=256;klen+=64)
			AES::testAES(type,klen);
	AES::demoAES();

	RSA::testRSA(128,200);
	RSA::demoRSA();
	
	RC4::testRC4(128,1024);
	RC4::demoRC4();
	return 0;
}