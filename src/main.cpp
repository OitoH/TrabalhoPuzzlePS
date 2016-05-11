
#include <iostream>
#include <chrono>

// MPI
#include <mpi.h>

// Puzzle and solver
#include "../include/BTtree.h"
#include "../include/puzzle.h"

using namespace std;

int main(int argc, char *argv[]) {

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get MPI number of processes
    int npes = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // Get MPI rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Input: aleatory puzzle?
    char isRandom = 'n';
    cout << "Voce deseja criar um puzzle aleatorio? ('s' ou 'n'): ";
    cin >> isRandom;

    // Check input
    while(isRandom != 's' && isRandom != 'n') {
        cout << "\nEntrada invalida, digite novamente ('s' ou 'n'): ";
        cin >> isRandom;
    }

    // Input: puzzle size
    unsigned int tam = -1;
    cout << "Entre com o tamanho do puzzle: ";
    cin >> tam;
    while(tam <= 0) {
        cout << "\nTamanho invalido, digite novamente: ";
        cin >> tam;
    }

    // Create puzzle
    puzzle *puzzleToSolve = nullptr;
    if(isRandom == 's') {
        puzzleToSolve = new puzzle(tam);

    } else {
        cout << "Digite os valores do puzzle em sequencia, sem virgula:" << endl;

        // Populate matrix
        vector<vector<uint_fast8_t>> elements;
        for(unsigned int i = 0; i < tam; ++i) {
            elements.push_back(vector<uint_fast8_t>());

            for(unsigned int j = 0; j < tam; ++j) {
                unsigned int temp;
                cin >> temp;
                elements[i].push_back(static_cast<uint_fast8_t>(temp));

                if(temp >= tam*tam) {
                    cout << "Entrada invalida, deve ser menor que o tamanho!" << endl;
                    MPI_Finalize();
                    return EXIT_FAILURE;
                }
            }
        }

        puzzleToSolve = new puzzle(elements);
    }

    // Check allocated puzzle
    if(puzzleToSolve == nullptr) {
        cout << "Erro inesperado de alocacao de memoria!" << endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Show puzzle
    cout << "\n\nQuebra cabeça: " << endl << puzzleToSolve->toString();

    // Check if puzzle is solvable
    if(!puzzleToSolve->check_solve()){
        cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
        MPI_Finalize();
        return EXIT_SUCCESS;
	} else {
        cout << "Caso solucionável!\n";
    }

    // Create solver
    BTtree solver(*puzzleToSolve);

    chrono::time_point<chrono::high_resolution_clock> beginTime, endTime;

    // Solve (and compute time)
    beginTime = chrono::high_resolution_clock::now();
	solver.startDeathRide();
    endTime = chrono::high_resolution_clock::now();

    // Show time
    chrono::duration<double> responseTime = chrono::duration_cast<chrono::duration<double>>(endTime - beginTime);
    cout << "Tempo de resposta: " << responseTime.count()  << " segundos." << endl;

    // Delete puzzle
    delete puzzleToSolve;

    // Finalize MPI
    MPI_Finalize();

	return 0;
}
