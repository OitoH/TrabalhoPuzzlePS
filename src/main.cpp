#include <iostream>
#include <BTtree.h>
#include <puzzle.h>

using namespace std;

int main(){
	int tam;

	cin >> tam;

	puzzle original(tam);

	/*
	puzzle original { // Caso teste: não solucinável.
			{0, 3},
			{1, 2}
	};
/*
	 puzzle original { // Caso teste: solucionado.
	 	{1, 2},
	 	{3, 0}
	 };
	 */
/*
	puzzle original { // Caso teste: solucionável, solcuionável em 1  movimento.
		{1, 2},
		{0, 3}
	 };
*/
/*
	puzzle original { // Caso teste: pior configuração, 6 passos.
		{0, 3},
		{2, 1}
	};
*/

	cout << "Quebra cabeça: \n" << original.toString();

	if(!original.check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	}

	BTtree solver(original);
	solver.startDeathRide();

	return 0;
}
