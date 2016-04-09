#include <iostream>
#include <BTtree.h>
#include <puzzle.h>

using namespace std;

int main(){
	int tam;

	cin >> tam;

	puzzle original(tam);

	cout << "Quebra cabeça gerado: \n" << original.toString();

	if(!original.check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	}

	BTtree solver(original);
	solver.startDeathRide();

	return 0;
}
