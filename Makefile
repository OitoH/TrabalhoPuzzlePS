CC = g++
CFLAGS = -g -std=c++11 -g #-Og -Wall

INCLUDE = -Iinclude/ -I/usr/include/
LIBS = -fopenmp
#LIBS += -lm

CL = $(wildcard src/*.cpp)    #cpp list
HL = $(wildcard include/*.hpp)    #header list
OL = $(patsubst src/%.cpp, obj/%.o, $(CL) )  #object list

all: $(OL); $(CC) $(CFLAGS) $(OL) $(LIBS) $(INCLUDE) -o main

obj/%.o: src/%.cpp; $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

run: ; ./main

clean: ; rm main obj/*.o