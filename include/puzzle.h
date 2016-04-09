#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <cmath>

using namespace std;

enum zero_movement
{
	ZERO_UP,
	ZERO_DOWN,
	ZERO_LEFT,
	ZERO_RIGHT,
	ZERO_NONE
};

class puzzle {

public:

	puzzle(int tam);

	puzzle(const puzzle &original);

	int properLine(int pieceNum);

	int properColumn(int pieceNum);

	int pieceNum(int column, int line);

	bool check_solve();

	int manhattan_dist();

	bool isMoveValid(enum zero_movement move);

	int move_zero(enum zero_movement dir);

	int inversion();

	string toString();

private:
	void manhattan_update(int line, int column);

	int tam;
	int line0, column0;
	vector< vector<int> > table;
	vector< vector<int> > distances;
};
