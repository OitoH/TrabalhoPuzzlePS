#include "../include/BTtree.h"
#include <omp.h>

BTtree::node::node(const puzzle &copy)
    : infos(copy), moviment(puzzle::ZERO_NONE), depth(0)
{

}

BTtree::node::node(node *lastNode, enum puzzle::zero_movement thisMovement)
    : infos(lastNode->infos), moviment(thisMovement)
{
    depth = lastNode->depth + 1;
    infos.move_zero(moviment);
}

BTtree::BTtree(const puzzle &original)
    : rootNode(new node(original))
{

}

BTtree::~BTtree() {
    delete rootNode;
}

bool BTtree::generateInitialNodes(unsigned mpi_npes) {

    const enum puzzle::zero_movement movements[4] = {
        puzzle::ZERO_UP,
        puzzle::ZERO_DOWN,
        puzzle::ZERO_LEFT,
        puzzle::ZERO_RIGHT
    };

    bool solved = false;

    // Insert initial node
    globalNodes.push_back(new node(rootNode->infos));

    // Gera nós até possuir no mínimo 3 nós por thread
    do {
        node *currentNode = globalNodes.front();
        globalNodes.pop_front();

        // Checa se já solucionou
        if(currentNode->infos.manhattan_dist() == 0) {
            solved = true;
            solution = currentNode;

        // Se não, realiza todos os movimentos possíveis no zero
        } else {
            for(int i=0; i<4; i++) {
                if(currentNode->moviment != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
                    globalNodes.push_back(new node(currentNode, movements[i]));
            }
            delete currentNode;

        }

    } while(solved==false && globalNodes.size() < 3*mpi_npes);

    return solved;
}

void BTtree::startDeathRide() {

    const enum puzzle::zero_movement movements[4] = {
		puzzle::ZERO_UP,
		puzzle::ZERO_DOWN,
		puzzle::ZERO_LEFT,
		puzzle::ZERO_RIGHT
	};

    int i;
    unsigned int threadsNum, myID;
    bool solved = false;
    deque<node *> globalNodes;
    vector<node *> unexploredNodes;
    node *currentNode, *solution = nullptr;

    #pragma omp parallel private(i, threadsNum, myID, unexploredNodes, currentNode), shared(globalNodes, solved, solution)
    {
        // Get OpenMP infos
        threadsNum = omp_get_num_threads();
        myID = omp_get_thread_num();

        // Uma thread qualquer (master) gera os nós iniciais para todas as threads
        #pragma omp single
        {
            globalNodes.push_back(new node(rootNode->infos));

            // Enquanto não existir ao menos 1 nó para cada thread ou não for possível nós suficientes para todas
            do {
                currentNode = globalNodes.front();
                globalNodes.pop_front();

                // Checa se já solucionou
                if(currentNode->infos.manhattan_dist() == 0) {
                    solved = true;
                    solution = currentNode;

                // Se não, realiza todos os movimentos possíveis no zero
                } else {

                    for(i = 0; i < 4; i++) {
                        if(currentNode->moviment != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
                            globalNodes.push_back(new node(currentNode, movements[i]));
                    }
                    delete currentNode;

                }

            } while(solved==false && globalNodes.size() < threadsNum);
        }

        // Se ainda não encontrou solução
        if(solved==false) {

            // Cada thread preenche a sua lista de prioridade
            for(i = globalNodes.size()-1-myID; i >= 0; i-=threadsNum)
                unexploredNodes.push_back(globalNodes[i]);

            // Garantir que todas as threads já formaram suas listas
            #pragma omp barrier

            // Limpa os nodes globais da thread master
            #pragma omp single
            globalNodes.clear();

            if(unexploredNodes.empty()==false) {
                make_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());

                // Inicia exploração.
                // Enquanto uma solução definitiva não foi encontrada.
                while(solved==false) {

                    // Explora o nó de maior prioridade.
                    currentNode = unexploredNodes.front();
                    pop_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
                    unexploredNodes.pop_back();

                    // Se a thread encontrou uma solução.
                    if (currentNode->infos.manhattan_dist() == 0) {
                        // Caso mais de uma thread encontre uma solução em tempo similar a melhor solução será escolhida.
                        #pragma omp critical
                        {
                            if(solved==false) {
                                solved = true;
                                solution = currentNode;

                            } else if(solution->depth > currentNode->depth) {
                                solution = currentNode;

                            } else {
                                delete currentNode;

                            }
                        }

                    } else {
                        // Realizar todos os movimentos possíveis no zero.
                        for (i = 0; i < 4; i++) {
                            if (currentNode->moviment != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i])) {
                                unexploredNodes.push_back(new node(currentNode, movements[i]));
                                push_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
                            }
                        }

                        // Deleta nó atual (já processado)
                        delete currentNode;
                    }

                }

                // Liberando memória
                while(unexploredNodes.empty()==false) {
                    delete unexploredNodes.back();
                    unexploredNodes.pop_back();
                }

            }
        }

        // Espera todas as threads
        #pragma omp barrier
    }

    // Exibe resultado
    cout << "Resultado:\tProfundidade:" << solution->depth << "\n" << solution->infos.toString() << endl;

    // Liberando memória alocada.
    delete solution;
}

BTtree::node *BTtree::getSolution() {
    return solution;

}

deque<BTtree::node *> *BTtree::generatedNodes() {
    return &globalNodes;
}

bool BTtree::node::priorityCalculator::operator() (node *lhs, node *rhs) const {
    return (lhs->infos.manhattan_dist() + lhs->depth/lhs->infos.getTam()) > (rhs->infos.manhattan_dist() + rhs->depth/lhs->infos.getTam());
}
