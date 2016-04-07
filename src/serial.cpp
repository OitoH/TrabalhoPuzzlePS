#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int posx(int k){ return floor((k-1)/n); }
int posy(int k){ return (k-1)%n; }
int num(int i, int j){ return n*i+j; }

class puzzle {

public:

	puzzle(int tam) {
		this.tam = tam;
	}

	void in() {}

	bool check_solve() {}

	int manhattan_dist() {
		int i, j;
		int d=0;
		for(i=0; i<n;i++){
			for(j=0;j<n;j++){
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

private:
	void manhattan_update(int x, int y) {
		distances[x][y] = abs(x-posx(table[x][y])) + abs(y-posy(table[x][y]));
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
