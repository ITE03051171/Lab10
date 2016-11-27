all: multithread04 client
multithread04: multithread04.c
gcc multithread04.c -o multithread04 -lpthread
client: client.o
gcc -o client client.o
client.o: client.c
gcc -c client.c
clean:
rm multithread03
