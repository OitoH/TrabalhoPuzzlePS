
#include <iostream>
#include <chrono>

// MPI
#include <mpi.h>

// OMP
#include <omp.h>

// Puzzle and solver
#include "../include/BTtree.h"
#include "../include/puzzle.h"

using namespace std;

#define CMD_UNDEFINED 0
#define CMD_INIT_OK 1
#define CMD_INIT_ABORT 2
#define CMD_STARTRESOLUTION 3
#define CMD_SOLVED 3

#define MASTER_RANK 0

void bcastInitOK() {
    int cmd = CMD_INIT_OK;
    MPI_Bcast(&cmd, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
}

void bcastInitAbort() {
    int cmd = CMD_INIT_ABORT;
    MPI_Bcast(&cmd, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
}

void bcastStartResolution() {
    int cmd = CMD_STARTRESOLUTION;
    MPI_Bcast(&cmd, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
}

void sendSolved(int sourceSolved, int npes) {
    for(int i=1; i<npes; i++) {
        if(i!=sourceSolved) {
            int cmd = CMD_SOLVED;
            MPI_Send(&cmd, 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
        }
    }
}

void startResolution(BTtree &solver, int rank, int npes) {
    MPI_Status status;

    // Shared
    int numProcessSolved = -1;
    int solutionDepth = 0;
    bool solved = false;

    // Start 2 threads
    bool keepRunning = true;
    #pragma omp parallel private(status), shared(solver, solved, keepRunning, numProcessSolved, solutionDepth) num_threads(2)
    {
        // Get OpenMP infos
        int omp_myID = omp_get_thread_num();

        // Swich based on thread
        if(omp_myID==0) {
            int cmd;

            MPI_Request request;
            if(rank==MASTER_RANK)
                MPI_Irecv(&cmd, 1, MPI_INTEGER, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            else
                MPI_Irecv(&cmd, 1, MPI_INTEGER, MASTER_RANK, 0, MPI_COMM_WORLD, &request);

            // Non-blocking wait
            while(solved==false) {
                int received = 0;
                MPI_Test(&request, &received, &status);
                if(received)
                    break;
            }

            // Check cmd
            if(cmd==CMD_SOLVED) {
                keepRunning = false;

                // If received solved, and it's master, inform all threads
                if(rank==MASTER_RANK) {
                    // Get source
                    numProcessSolved = status.MPI_SOURCE;

                    // Receive solution depth
                    MPI_Recv(&solutionDepth, 1, MPI_INTEGER, numProcessSolved, 0, MPI_COMM_WORLD, &status);

                    // Inform all processes
                    sendSolved(numProcessSolved, npes);
                }
            }

        } else {
            // Start resolution
            solved = solver.startDeathRide(&keepRunning);

            // If solved
            if(solved) {
                // If master, inform all slaves
                if(rank==MASTER_RANK) {
                    numProcessSolved = MASTER_RANK;
                    solutionDepth = solver.getSolutionDepth();
                    sendSolved(MASTER_RANK, npes);

                // If slave, inform master
                } else {
                    MPI_Request request;

                    // Inform
                    int cmd = CMD_SOLVED;
                    MPI_Isend(&cmd, 1, MPI_INTEGER, MASTER_RANK, 0, MPI_COMM_WORLD, &request);

                    // Send solution depth
                    int depth = solver.getSolutionDepth();
                    MPI_Isend(&depth, 1, MPI_INTEGER, MASTER_RANK, 0, MPI_COMM_WORLD, &request);
                }
            }

        }

        #pragma omp barrier
    }

    // Print which process solved the puzzle
    if(rank==MASTER_RANK) {
        std::cout << "Processo #" << numProcessSolved << " resolveu o puzzle! (profundidade: " << solutionDepth << ")\n";
    }

}

int main(int argc, char *argv[]) {
    MPI_Status status;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get MPI number of processes
    int npes = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &npes);

    // Get MPI rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Master process: get user input and start distribution to slaves
    if(rank==0) {

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
            vector<vector<puzzle::element_type>> elements;
            for(unsigned int i = 0; i < tam; ++i) {
                elements.push_back(vector<puzzle::element_type>());

                for(unsigned int j = 0; j < tam; ++j) {
                    unsigned int temp;
                    cin >> temp;
                    elements[i].push_back(static_cast<puzzle::element_type>(temp));

                    if(temp >= tam*tam) {
                        cout << "Entrada invalida, deve ser menor que o tamanho!" << endl;
                        bcastInitAbort();
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
            bcastInitAbort();
            MPI_Finalize();
            return EXIT_FAILURE;
        }

        // Show puzzle
        cout << "\nQuebra cabeça: " << endl << puzzleToSolve->toString();

        // Check if puzzle is solvable
        if(!puzzleToSolve->check_solve()){
            cout << "Nao ha solucao para esse caso, chupa essa manga!" << endl;
            bcastInitAbort();
            MPI_Finalize();
            return EXIT_SUCCESS;
        } else {
            cout << "Caso solucionável!\n\n";
        }

        cout << "Procurando solução...\n\n";

        // Create solver
        BTtree solver(*puzzleToSolve);

        chrono::time_point<chrono::high_resolution_clock> beginTime, endTime;
        beginTime = chrono::high_resolution_clock::now();

        // Generate initial nodes for processes
        bool solved = solver.generateInitialNodes(npes);

        // Se nao resolveu, confirma e dispara os processos
        if(solved==false) {

            // Confirma
            bcastInitOK();

            // Recupera os nodes
            deque<BTtree::node*>* globalNodes = solver.generatedNodes();

            // Envia um nó para cada processo
            for(int i=0; i<npes-1; i++) {
                int dest = i+1;

                // Envia tam
                MPI_Send(&tam, 1, MPI_INTEGER, dest, 0, MPI_COMM_WORLD);

                // Envia o nó
                BTtree::node *node = globalNodes->back();
                globalNodes->pop_back();

                puzzle::element_type *puzz = node->infos.getPuzzle();
                MPI_Send(puzz, tam*tam, MPI_INTEGER, dest, 0, MPI_COMM_WORLD);
            }

            // Dispara resolução
            bcastStartResolution();

            // Inicia resolução
            startResolution(solver, rank, npes);

        // Se resolveu
        } else {
            bcastInitAbort();
        }

        endTime = chrono::high_resolution_clock::now();

        // Show time
        chrono::duration<double> responseTime = chrono::duration_cast<chrono::duration<double>>(endTime - beginTime);
        cout << "Tempo de resposta: " << responseTime.count()  << " segundos." << endl;

        // Delete puzzle
        delete puzzleToSolve;

    // Slaves processes: receives nodes and start processing
    } else {

        // Recv initial confirmation
        int cmd = 0;
        MPI_Bcast(&cmd, 1, MPI_INTEGER, MASTER_RANK, MPI_COMM_WORLD);
        if(cmd==CMD_INIT_OK) {

            // Receive number of nodes (numNodes)
            int tam=0;
            MPI_Recv(&tam, 1, MPI_INTEGER, MASTER_RANK, 0, MPI_COMM_WORLD, &status);

            // Receive node
            puzzle::element_type puzz[tam*tam];
            MPI_Recv(puzz, tam*tam, MPI_INTEGER, MASTER_RANK, 0, MPI_COMM_WORLD, &status);

            // Create puzzle with node
            puzzle *p = new puzzle(puzz, tam);

            // Create solver
            BTtree solver(*p);

            // Wait for command to start resolution
            MPI_Bcast(&cmd, 1, MPI_INTEGER, MASTER_RANK, MPI_COMM_WORLD);
            if(cmd==CMD_STARTRESOLUTION)
                startResolution(solver, rank, npes);

        }

    }

    // Finalize MPI
    MPI_Finalize();

    return EXIT_SUCCESS;
}
