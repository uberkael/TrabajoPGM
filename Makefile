CC=gcc
OPCIONES=-W -Wall #-g
THREADS=-lpthread
monoproceso=PGMmonoproceso
prueba2=reloj_prueba2
lento=enlentecedor

all: clean monoproceso
monoproceso:
	$(CC) $(OPCIONES) $(monoproceso).c -o $(monoproceso) $(THREADS)
clean:
	rm -f ${monoproceso}
