# Prueba mpi

#variable compilador
CC=mpiCC
#optimizaciones
#CFLAGS=-DDEBUG -O3 #-D (pasar variables) -O[0,1,2,3](optimizacion)
CFLAGS=-g -DDEBUG -std=c++11 -lpthread
LDLIBS=
INCLUDE=-I. -I./include
EXEC=practica1
OBJS=utilities.o nodo.o arbol.o main.o terminal.o harddisc.o slave.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDLIBS) $(OBJS) -o $(EXEC)

%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
#para cada fichero.o busca su equivalente .cpp // -c para no linkar //$< = %.cpp //  $@ = %.o

clean:
	rm *.o	
	rm $(EXEC)
