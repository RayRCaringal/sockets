all:DUMBclient.c DUMBserver.c
	gcc -o DUMBclient DUMBclient.c -lpthread
	gcc -o DUMBserver DUMBserver.c -lpthread

clean:
	rm DUMBclient DUMBserver