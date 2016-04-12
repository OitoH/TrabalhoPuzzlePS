#include <iostream>
#include "../include/BTtree.h"
#include "../include/puzzle.h"

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
        */

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
        
        // SOLVABLE!
    /*
        puzzle original {
            {11, 6, 0, 8},
            {15, 4, 12, 7},
            {5, 9, 3, 2},
            {1, 14, 10, 13}            
        };
*/
        
        
        /* // UNSOLVABLE!
        puzzle original {
            {15, 11, 4, 8},
            {12, 5, 3, 7},
            {9, 1, 10, 0},
            {6, 14, 13, 2}            
        };
        */
        
        // SOLVABLE
    /*
        puzzle original {
            {1, 0, 2},
            {3, 6, 5},
            {8, 7, 4}
        };
        */
        
        /* // UNSOLVABLE
        puzzle original {
            {3, 4, 8},
            {7, 2, 0},
            {1, 6, 5}
        };
        */
/*
        puzzle original { //4 -> 1.14s
            {2, 9, 11, 12,},
            {5, 14, 0, 1,},
            {8, 6, 3, 7,},
            {10, 13, 4, 15}
        };
*/
/*
    puzzle original { //5 -> 23.5s
        {23, 12, 0,	10,	18},
        {7,	9,	13,	17,	4},
        {3,	21,	24,	20,	8},
        {11, 16, 1,	15,	19},
        {6,	2,	5,	22,	14}
    };
*/
	cout << "Quebra cabeça: \n" << original.toString();

	if(!original.check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	} else {
            cout << "Solvable!\n";
        }
    

	BTtree solver(original);
	solver.startDeathRide();

	return 0;
}
