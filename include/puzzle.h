#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <initializer_list>
#include <random>
#include <chrono>
#include <string>
#include <sstream>

#include <cmath>

using namespace std;

class puzzle {

public:

    //typedef uint_fast8_t element_type;
    typedef int element_type;

    enum zero_movement
    {
		ZERO_UP,
		ZERO_DOWN,
		ZERO_LEFT,
		ZERO_RIGHT,
		ZERO_NONE
	};

	static enum zero_movement oppositeMovement(enum zero_movement dir);

    puzzle(const initializer_list<initializer_list<element_type>>& elementList);

    puzzle(element_type *puzz, int tam);

    puzzle(const vector<vector<element_type>>& elements);

	puzzle(int tam);

	puzzle(const puzzle &original);

    ~puzzle();

    int getTam();

	int properLine(int pieceNum);

	int properColumn(int pieceNum);

	int pieceNum(int column, int line);

	bool check_solve();

	bool isMoveValid(enum zero_movement move);

	int manhattan_dist() const;

	int move_zero(enum zero_movement dir);

	int inversion();

    string toString();

    element_type* getPuzzle() { return storage; }

private:
	void compute_manhattan_dist();
	void manhattan_update(int line, int column);

	int tam;
	int line0, column0;
    int totalDistance;
    element_type **table;
    element_type *storage;
    element_type **distances;
};
