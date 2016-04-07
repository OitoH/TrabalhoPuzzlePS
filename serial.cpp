#include <iostream>
#include <vector>

using namespace std;

class puzzle {
	int tam;
	int posx0, posy0;
	vector<vector<int>> table;
	vector<vector<int>> distances;
	
	puzzle(int tam) {
		this.tam = tam;
	}

	void in() {}

	bool check_solve() {}

	int manhattan_dist() {}

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

	private void manhattan_update(int x, int y) {}
}

int main(){
	int tam;
	puzzle original;

	cin >> tam;

	original = new puzzle(tam);
	original.in();

	if(!original.check_solve()){
		cout("Nao ha solucao para esse caso. :(\n");
		return 0;
	}

	return 0;
}
