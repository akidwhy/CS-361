LIBS = -L/usr/local/lib -lpcap

all: lib361.so main

lib361.so: 2.c header.h
	gcc -c -Wall -Werror -fpic 2.c
	gcc -shared -o lib361.so 2.o

main: main.c lib361.so
	gcc -L$(PWD) main.c -o main -l2