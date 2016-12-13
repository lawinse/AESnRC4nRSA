.PHONY: all
all: main
main: main.o RC4.o RSA.o AES.o
	g++ -std=c++11 -o main main.o RC4.o RSA.o AES.o
main.o: main.cpp 
	g++ -std=c++11 -c main.cpp -o main.o
RC4.o: RC4.cpp
	g++ -std=c++11 -c RC4.cpp -o RC4.o
AES.o: AES.cpp
	g++ -std=c++11 -c AES.cpp -o AES.o
RSA.o: RSA.cpp
	g++ -std=c++11 -c RSA.cpp -o RSA.o
.PHONY: clean
clean:
	-rm main *.o