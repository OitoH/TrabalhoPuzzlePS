CC = g++
CFLAGS = -g -std=c++11 -g -Wall #-Og -Wall

INCLUDE = -Iinclude/ -I/usr/include/
LIBS = -fopenmp
#LIBS += -lm

CL = $(wildcard src/*.cpp)    #cpp list
HL = $(wildcard include/*.h)    #header list
OL = $(patsubst src/%.cpp, obj/%.o, $(CL) )  #object list

all: $(OL); $(CC) $(CFLAGS) $(OL) $(LIBS) $(INCLUDE) -o main

obj:
	mkdir -p obj

obj/%.o: src/%.cpp obj; $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run: ; ./main

clean: ; rm $(wildcard obj/*.o) main
