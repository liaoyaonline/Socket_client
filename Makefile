#定义编译器
all: client_daiqf.o cClient.o cJSON.o
	gcc -o bin/cClient_daiqf client_daiqf.o cClient.o cJSON.o -g -lm -lcrypto
client_daiqf.o:
	gcc -c -I./include ./src/client_daiqf.c
cClient.o:
	gcc -c -I./include ./include/cClient/cClient.c
cJSON.o:
	gcc -c -I./include ./include/cJSON/cJSON.c
.PHONY:clean
clean:
	rm ./*.o
