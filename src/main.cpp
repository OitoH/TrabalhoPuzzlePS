#include <iostream>
#include "../include/BTtree.h"
#include "../include/puzzle.h"
#include <chrono>

using namespace std;

int main(){
    auto beginTime = chrono::high_resolution_clock::now();
	unsigned int tam;
    char isRandom;
    puzzle *original = nullptr;

    cout << "Voce deseja criar um puzzle aleatorio? ('s' ou 'n')" << endl;

    cin >> isRandom;

    while(isRandom != 's' && isRandom != 'n')
    {
        cout << "\nEntrada invalida, digite novamente ('s' ou 'n')" << endl;

        cin >> isRandom;
    }

    tam = -1;

    cout << "Entre com o tamanho do puzzle" << endl;

    cin >> tam;

    while(tam <= 0)
    {
        cout << "\nTamanho invalido, digite novamente" << endl;
        cin >> tam;
    }

    if(isRandom == 's')
    {
        original = new puzzle(tam);
    }
    else
    {
        cout << "Digite os valores do puzzle em sequencia sem virgula" << endl;

        vector<vector<uint_fast8_t>> elementos;

        for(unsigned int i = 0; i < tam; ++i)
        {
            elementos.push_back(vector<uint_fast8_t>());
            for(unsigned int j = 0; j < tam; ++j)
            {
                unsigned int temp;
                cin >> temp;
                elementos[i].push_back(static_cast<uint_fast8_t>(temp));

                if(temp >= tam*tam)
                {
                    cout << "Entrada invalida, deve ser menor que o tamanho" << endl;

                    return -1;
                }
            }
        }

        original = new puzzle(elementos);
    }

    if(original == nullptr)
    {
        cout << "Erro inesperado de alocacao de memoria" << endl;
        return -1;
    }

	cout << "\n\nQuebra cabeça: " << endl << original->toString();

	if(!original->check_solve()){
		cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
		return 0;
	} else {
            cout << "Solvable!\n";
    }


	BTtree solver(*original);
	solver.startDeathRide();

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> responseTime = chrono::duration_cast<chrono::duration<double>>(endTime - beginTime);
    cout << "Tempo de resposta: " << responseTime.count()  << " segundos." << endl;

    delete original;

	return 0;
}
