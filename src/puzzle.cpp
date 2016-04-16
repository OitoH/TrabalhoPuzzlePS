#include "../include/puzzle.h"
#include <omp.h>

enum puzzle::zero_movement puzzle::oppositeMovement(enum puzzle::zero_movement dir)
{
	switch(dir)
	{
		case puzzle::ZERO_UP:
			return puzzle::ZERO_DOWN;
		case puzzle::ZERO_DOWN:
			return puzzle::ZERO_UP;
		case puzzle::ZERO_LEFT:
			return puzzle::ZERO_RIGHT;
		case puzzle::ZERO_RIGHT:
			return puzzle::ZERO_LEFT;
		default:
			break;
	}
	return puzzle::ZERO_NONE;
}

puzzle::puzzle(const initializer_list<initializer_list<uint_fast8_t>>& elementList)
    : tam(elementList.size())
    , table(new uint_fast8_t*[tam])
    , distances(new uint_fast8_t*[tam])
{
    int j, i = 0;

	for(auto& it: elementList)
	{
		if (it.size() != tam)
			throw -1;

        table[i] = new uint_fast8_t[tam];
        distances[i] = new uint_fast8_t[tam];

		j = 0;
		for(auto piece: it)
		{
			if (piece == 0)
			{
				line0 = i;
				column0 = j;
			}
			table[i][j] = piece;
			++j;
		}
		++i;
	}
	compute_manhattan_dist();
}

puzzle::puzzle(int tam)
    : table(new uint_fast8_t*[tam])
    , distances(new uint_fast8_t*[tam])
{
    int i, j;
    uint_fast8_t aux;
    vector<uint_fast8_t> random_pieces(tam * tam);

	this->tam = tam;

	// Aleatorizar peças.
	for (i = tam * tam - 1; i > -1; --i)
		random_pieces[i] = i;

    shuffle(random_pieces.begin()
		    , random_pieces.end()
		    , default_random_engine(chrono::system_clock::now().time_since_epoch().count())
		    );

	// Aloca espaço no vetor (opcional, mas é mais rápido).
	for (i = 0; i < tam; ++i)
	{
        table[i] = new uint_fast8_t[tam];
        distances[i] = new uint_fast8_t[tam];
	}

	// Inicializa o quebra-cabeça com valores aleatórios.
	for (i = 0; i < tam; ++i)
	{
		for (j = 0; j < tam; ++j)
		{
			aux = random_pieces[i * tam + j];
			table[i][j] = aux;
			if (aux == 0)
			{
				line0 = i;
				column0 = j;
			}
		}
	}
	compute_manhattan_dist();
}

puzzle::puzzle(const puzzle &original)
    : tam(original.tam)
    , line0(original.line0)
    , column0(original.column0)
    , totalDistance(0)
    , table(new uint_fast8_t*[tam])
    , distances(new uint_fast8_t*[tam])
{
#pragma omp parallel for
	for (int i = 0; i < tam; ++i)
	{
        table[i] = new uint_fast8_t[tam];
        distances[i] = new uint_fast8_t[tam];
		for (int j = 0; j < tam; ++j)
		{
			table[i][j] = original.table[i][j];
			distances[i][j] = original.distances[i][j];
		}
    }
    compute_manhattan_dist();
}

puzzle::~puzzle()
{
    int i;
    for(i = 0; i < tam; i++)
    {
        delete [] table[i];
        delete [] distances[i];
    }
    delete [] table;
    delete [] distances;
}

int puzzle::properLine(int pieceNum){ return (pieceNum - 1) / tam; }

int puzzle::properColumn(int pieceNum){ return (pieceNum - 1) % tam ; }

int puzzle::pieceNum(int column, int line){ return tam * line + column + 1; }

bool puzzle::check_solve() {
	return ( ((tam % 2 == 1) && (inversion() % 2 == 0)) ||
		     ((tam % 2 == 0) && (((tam - line0) % 2 == 1) == (inversion() % 2 == 0)))
		   ); // FORMULA DE SOLUCIONABILIDADE
}

void puzzle::compute_manhattan_dist() {
	int i, j;
    totalDistance = 0;
	for(i = 0; i < tam; i++){
		for(j = 0;j < tam; j++){
			manhattan_update(i, j);
            totalDistance += distances[i][j];
		}
	}
}

int puzzle::manhattan_dist() const
{
    return totalDistance;
}

bool puzzle::isMoveValid(enum zero_movement move)
{
	switch(move)
	{
		case ZERO_UP:
			return line0 != 0;
		case ZERO_DOWN:
			return line0 != tam - 1;
		case ZERO_LEFT:
			return column0 != 0;
		case ZERO_RIGHT:
			return column0 != tam - 1;
		default:
		break;
	}
	return false;
}

int puzzle::move_zero(enum zero_movement dir) {
	int num;

	switch(dir)
	{
		case ZERO_UP:
			if(line0 == 0) return -1;
			num = table[line0-1][column0];
			table[line0][column0] = num;
			line0--;
			table[line0][column0] = 0;
			manhattan_update(line0+1, column0);
		break;
		case ZERO_DOWN:
			if(line0 == tam - 1) return -1;
			num = table[line0+1][column0];
			table[line0][column0] = num;
			line0++;
			table[line0][column0] = 0;
			manhattan_update(line0-1, column0);
		break;
		case ZERO_LEFT:
			if(column0 == 0) return -1;
			num = table[line0][column0-1];
			table[line0][column0] = num;
			column0--;
			table[line0][column0] = 0;
			manhattan_update(line0, column0+1);
		break;
		case ZERO_RIGHT:
			if(column0 == tam - 1) return -1;
			num = table[line0][column0+1];
			table[line0][column0] = num;
			column0++;
			table[line0][column0] = 0;
			manhattan_update(line0, column0-1);
		break;
		default:
			return -1;
	}
	manhattan_update(line0, column0);
	return 0;
}

int puzzle::inversion(){
	int i, j, preceding, successor;
	int inv = 0;
	for(i = 1; i < tam * tam; i++) {
		preceding = table[properLine(i)][properColumn(i)];
		for(j = i + 1; j <= tam * tam; j++) {
			successor = table[properLine(j)][properColumn(j)];
			if (successor != 0 && preceding > successor)
				inv++;
		}
	}
	std::cout << "inversions: " << inv << "\n";
	return inv;
}

string puzzle::toString(){
	stringstream stream;
	int i, j;

	for(i = 0; i < tam; ++i)
	{
        stream << "{";
        stream << (int)table[i][0];
		for(j = 1; j < tam; ++j)
            stream << ",\t" << (int)table[i][j];
        stream << "}\n";
	}

	return stream.str();
}

void puzzle::manhattan_update(int line, int column) {
    uint_fast8_t value = table[line][column];
    uint_fast8_t formerDistance = distances[line][column];

	if (value == 0)
		distances[line][column] = 0;
	else
		distances[line][column] = abs(line-properLine(value)) + abs(column-properColumn(value));

    totalDistance += distances[line][column] - formerDistance;
}

int puzzle::getTam()
{
    return tam;
}
