#include <puzzle.h>

puzzle::puzzle(int tam)
	: table(tam)
	, distances(tam)
{
	int i, j, aux;
	vector<int> random_pieces(tam * tam);

	this->tam = tam;

	// Aleatorizar peças.
	for (i = tam * tam - 1; i > -1; --i)
		random_pieces[i] = i;
	random_shuffle(random_pieces.begin(), random_pieces.end());

	// Aloca espaço no vetor (opcional, mas é mais rápido).
	for (i = 0; i < tam; ++i)
	{
		table[i].reserve(tam);
		distances[i].reserve(tam);
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
}

puzzle::puzzle(const puzzle &original)
	: table(original.table)
	, distances(original.distances)
{
	tam = original.tam;
	line0 = original.line0;
	column0 = original.column0;
}

int puzzle::properLine(int pieceNum){ return floor((pieceNum - 1) / tam); }

int puzzle::properColumn(int pieceNum){ return (pieceNum - 1) % tam ; }

int puzzle::pieceNum(int column, int line){ return tam * line + column + 1; }

bool puzzle::check_solve() {
	return ( ((tam % 2 == 1) && (inversion() % 2 == 0)) ||
		     ((tam % 2 == 0) && ((tam - line0) % 2 == 1) == (inversion() % 2 == 0))
		   ); // FORMULA DE SOLUCIONABILIDADE
}

int puzzle::manhattan_dist() {
	int i, j;
	int d=0;
	for(i=0; i<tam;i++){
		for(j=0;j<tam;j++){
			manhattan_update(i, j);
			d+=distances[i][j];
		}
	}
	return d;
}

bool puzzle::isMoveValid(enum zero_movement move)
{
	switch(move)
	{
		case ZERO_UP:
			return line0 == 0;
		case ZERO_DOWN:
			return line0 == tam - 1;
		case ZERO_LEFT:
			return column0 == 0;
		case ZERO_RIGHT:
			return column0 == tam - 1;
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
	return 0;
}

int puzzle::inversion(){
	int i, j;
	int auxi, auxj;
	int inv = 0;
	for(i=0; i<tam*tam; i++){
		for(j=i; j<tam*tam; j++){
			auxj = table[properLine(j)][properColumn(j)];
			auxi = table[properLine(i)][properColumn(j)];
			if(auxj<auxi && auxj!=0){
				inv++;
			}
		}
	}
	return inv;
}

string puzzle::toString(){
	stringstream stream;
	int i, j;

	for(i = 0; i < tam; ++i)
	{
		stream << table[i][0];
		for(j = 1; j < tam; ++j)
			stream << "\t" << table[i][j];
		stream << "\n";
	}

	return stream.str();
}

void puzzle::manhattan_update(int line, int column) {
	distances[line][column] = abs(line-properLine(table[line][column])) + abs(column-properColumn(table[line][column]));
}
