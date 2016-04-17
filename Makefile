CC = g++
CFLAGS = -fopenmp -g -std=c++11 -Wall #-Og -Wall

INCLUDE = -Iinclude/ -I/usr/include/
LIBS =
#LIBS += -lm

CL = $(wildcard src/*.cpp)    #cpp list
HL = $(wildcard include/*.h)    #header list
OL = $(patsubst src/%.cpp, obj/%.o, $(CL) )  #object list

all: $(OL); $(CC) $(CFLAGS) $(OL) $(LIBS) $(INCLUDE) -o main

obj:
	mkdir -p obj

obj/%.o: src/%.cpp obj; $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run: ; ./main

clean: ; rm -f $(wildcard obj/*.o) main
