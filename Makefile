#CC = g++
CC = mpic++
CFLAGS = -fopenmp -g -std=c++11 -Wall  #-Og -Wall

INCLUDE = -Iinclude/ -I/usr/include/
LIBS =
#LIBS += -lm

CL = $(wildcard src/*.cpp)    #cpp list
HL = $(wildcard include/*.h)    #header list
OL = $(patsubst src/%.cpp, obj/%.o, $(CL) )  #object list

all: main

main: $(OL); $(CC) $(CFLAGS) $(OL) $(LIBS) $(INCLUDE) -o main

obj:
	mkdir -p obj

obj/%.o: src/%.cpp obj; $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

GeradorDeTestes:
	qmake GeradorTestes/GeradorTestes.pro -r -spec linux-g++ -o GeradorTestes/Makefile
	$(MAKE) -C GeradorTestes
	mv GeradorTestes/GeradorTestes ./GeradorDeTestes

.PHONY: puzzle
puzzle: main

.PHONY: run
run: ; mpirun -np 40 --bind-to none --hostfile hostfile main

.PHONY: clean
clean:
	rm -f $(wildcard obj/*.o) main
	$(MAKE) -C GeradorTestes $@
	rm -f GeradorDeTestes

.PHONY: gerador
gerador: GeradorDeTestes
