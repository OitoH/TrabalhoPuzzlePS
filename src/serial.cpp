#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class puzzle {

public:

	int properLine(int pieceNum){ return floor((pieceNum - 1) / tam); }

	int properColumn(int pieceNum){ return (pieceNum - 1) % tam ; }

	int pieceNum(int column, int line){ return tam * line + column + 1; }

	puzzle(int tam) {
		this->tam = tam;
	}

	void in() {}

	bool check_solve() {
		return ( ((tam % 2 == 1) && (inversion() % 2 == 0)) ||
			     ((tam % 2==0) && ((tam - posx0) % 2 == 1) == (inversion() % 2 == 0))
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
			if(posy0 == 0) return -1;
			num = table[posx0][posy0-1];
			table[posx0][posy0] = num;
			posy0--;
			table[posx0][posy0] = 0;
			manhattan_update(posx0, posy0+1);
		}
		else if(dir == 'd') {
			if(posy0 == 0) return -1;
			num = table[posx0][posy0+1];
			table[posx0][posy0] = num;
			posy0++;
			table[posx0][posy0] = 0;
			manhattan_update(posx0, posy0-1);
		}
		else if(dir == 'l') {
			if(posy0 == 0) return -1;
			num = table[posx0-1][posy0];
			table[posx0][posy0] = num;
			posx0--;
			table[posx0][posy0] = 0;
			manhattan_update(posx0+1, posy0);
		}
		else if(dir == 'r') {
			if(posy0 == 0) return -1;
			num = table[posx0+1][posy0];
			table[posx0][posy0] = num;
			posy0++;
			table[posx0][posy0] = 0;
			manhattan_update(posx0-1, posy0);
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
	void manhattan_update(int x, int y) {
		distances[x][y] = abs(x-properLine(table[x][y])) + abs(y-properColumn(table[x][y]));
	}

	int tam;
	int posx0, posy0;
	vector<vector<int>> table;
	vector<vector<int>> distances;
};

int main(){
	int tam;
	puzzle original;

	cin >> tam;

	original = new puzzle(tam);
	original.in();

	if(!original.check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	}

	//TODO: Programa aqui

	return 0;
}
