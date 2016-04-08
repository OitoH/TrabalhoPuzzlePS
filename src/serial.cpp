#include <algorithm>
#include <iostream>
#include <vector>

#include <cmath>

using namespace std;

class puzzle {

public:

	int properLine(int pieceNum){ return floor((pieceNum - 1) / tam); }

	int properColumn(int pieceNum){ return (pieceNum - 1) % tam ; }

	int pieceNum(int column, int line){ return tam * line + column + 1; }

	puzzle(int tam) : table(tam), distances(tam) {
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

	void in() {}

	bool check_solve() {
		return ( ((tam % 2 == 1) && (inversion() % 2 == 0)) ||
			     ((tam % 2 == 0) && ((tam - line0) % 2 == 1) == (inversion() % 2 == 0))
			   ); // FORMULA DE SOLUCIONABILIDADE
	}

	int manhattan_dist() {
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

	int move_zero(char dir) {
		int num;

		if(dir == 'u') {
			if(line0 == 0) return -1;
			num = table[line0-1][column0];
			table[line0][column0] = num;
			line0--;
			table[line0][column0] = 0;
			manhattan_update(line0+1, column0);
		}
		else if(dir == 'd') {
			if(line0 == tam - 1) return -1;
			num = table[line0+1][column0];
			table[line0][column0] = num;
			line0++;
			table[line0][column0] = 0;
			manhattan_update(line0-1, column0);
		}
		else if(dir == 'l') {
			if(column0 == 0) return -1;
			num = table[line0][column0-1];
			table[line0][column0] = num;
			column0--;
			table[line0][column0] = 0;
			manhattan_update(line0, column0+1);
		}
		else if(dir == 'r') {
			if(column0 == tam - 1) return -1;
			num = table[line0][column0+1];
			table[line0][column0] = num;
			column0++;
			table[line0][column0] = 0;
			manhattan_update(line0, column0-1);
		}

		return 0;
	}

	int inversion(){
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

private:
	void manhattan_update(int line, int column) {
		distances[line][column] = abs(line-properLine(table[line][column])) + abs(column-properColumn(table[line][column]));
	}

	int tam;
	int line0, column0;
	vector<vector<int>> table;
	vector<vector<int>> distances;
};

int main(){
	int tam;
	puzzle original;

	cin >> tam;

	original = puzzle(tam);
	original.in();

	if(!original.check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	}

	//TODO: Programa aqui

	return 0;
}
