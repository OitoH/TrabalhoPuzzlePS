#include "../include/puzzle.h"

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

puzzle::puzzle(const initializer_list<initializer_list<int>>& elementList)
    : tam(elementList.size())
    , table(new int*[tam])
    , distances(new int*[tam])
{
    int j, i = 0;

    unordered_set<int> numbersUsed;
    numbersUsed.reserve(tam * tam);

	for(auto& it: elementList)
	{
        if (it.size() != tam) // Se a matriz não for n x n.
			throw -1;

        table[i] = new int[tam];
        distances[i] = new int[tam];

		j = 0;
		for(auto piece: it)
        {
			if (piece == 0)
			{
				line0 = i;
				column0 = j;
			}
            else if (piece < 0 || piece >= tam * tam || numbersUsed.insert(piece).second == false) // Se o número da peça for inválida ou a peça já estiver no conjunto de peças usadas.
                throw -1;
			table[i][j] = piece;
			++j;
		}
		++i;
	}
	compute_manhattan_dist();
}

puzzle::puzzle(int desiredTam)
    : tam(desiredTam)
    , table(new int*[tam])
    , distances(new int*[tam])
{
	int i, j, aux;
    vector<int> random_pieces(tam * tam);

	// Aleatorizar peças.
	for (i = tam * tam - 1; i > -1; --i)
		random_pieces[i] = i;
	shuffle(random_pieces.begin()
		    , random_pieces.end()
		    , default_random_engine(chrono::system_clock::now().time_since_epoch().count())
		    );

	for (i = 0; i < tam; ++i)
	{
        table[i] = new int[tam];
        distances[i] = new int[tam];
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
    , distance(original.distance)
    , table(new int*[tam])
    , distances(new int*[tam])
{
	for (int i = 0; i < tam; ++i)
	{
        table[i] = new int[tam];
        distances[i] = new int[tam];
		for (int j = 0; j < tam; ++j)
		{
            table[i][j] = original.table[i][j];
            distances[i][j] = original.distances[i][j];
		}
    }
}

puzzle::~puzzle()
{
    for(int i = 0; i < tam; ++i)
    {
        delete table[i];
        delete distances[i];
    }
    delete table;
    delete distances;
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
	distance = 0;
	for(i = 0; i < tam; i++){
		for(j = 0;j < tam; j++){
			manhattan_update(i, j);
			distance += distances[i][j];
		}
	}
}

int puzzle::manhattan_dist() const
{
	return distance;
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
    int i, j;
    int preceding, successor;
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
    int value = table[line][column];
    int formerDistance = distances[line][column];

	if (value == 0)
		distances[line][column] = 0;
	else
		distances[line][column] = abs(line-properLine(value)) + abs(column-properColumn(value));

	distance += distances[line][column] - formerDistance;
}

int puzzle::getTam()
{
    return tam;
}
