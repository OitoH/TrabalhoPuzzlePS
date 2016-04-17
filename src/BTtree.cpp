#include "../include/BTtree.h"
#include <algorithm>
#include <deque>
#include <omp.h>

BTtree::node::node(const puzzle &copy)
: infos(copy)
, movement(puzzle::ZERO_NONE)
, depth(0)
{}

BTtree::node::node(node *lastNode, enum puzzle::zero_movement thisMovement)
    : infos(lastNode->infos)
	, movement(thisMovement)
{
    depth = lastNode->depth + 1;
	infos.move_zero(movement);
}

BTtree::BTtree(const puzzle &original)
: rootNode(new node(original))
{}

void BTtree::startDeathRide()
{
    unsigned int threadsNum, myID;
    int i;
    
    const enum puzzle::zero_movement movements[4] = {
		puzzle::ZERO_UP,
		puzzle::ZERO_DOWN,
		puzzle::ZERO_LEFT,
		puzzle::ZERO_RIGHT
	};

    bool notSolved = true;
    deque<node *> globalNodes;
    vector<node *> unexploredNodes;
    node *currentNode, *solution = nullptr;

#pragma omp parallel private(i, threadsNum, myID, unexploredNodes, currentNode), shared(globalNodes, notSolved, solution)
{
    threadsNum = omp_get_num_threads();
    myID = omp_get_thread_num();

    // TODO: Paralelizar criação dos nós iniciais.
    // Uma thread qualquer gera os nós iniciais para todas as threads.
    #pragma omp single
    {
        currentNode = new node(rootNode->infos);

        // Enquanto não existir ao menos 1 nó para cada thread.
        while(globalNodes.size() < threadsNum)
        {
            // Realizar todos os movimentos possíveis no zero.
            for (i = 0; i < 4; i++)
                if (currentNode->movement != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
                    globalNodes.push_back(new node(currentNode, movements[i]));

            delete currentNode;

            currentNode = globalNodes.front();
            globalNodes.pop_front();
            //cout << "Next node\nDepth: " << currentNode->depth << " Manhattan: " << currentNode->infos.manhattan_dist() << "\n" << currentNode->infos.toString() << endl;
        }
    }

    // DEBUG
#pragma omp single nowait
    cout << "Not solved: " << notSolved << "\nNós gerados: " << globalNodes.size() << "\nThreads: " << threadsNum << endl;

    // Cada thread preenche a sua lista de prioridade.
    for(i = globalNodes.size() - 1 - myID; i > -1; i -= threadsNum)
        unexploredNodes.push_back(globalNodes[i]);

    make_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
    #pragma omp barrier // Garante que todas as threads já criaram suas listas.


    // Inicia exploração.
    // Enquanto uma solução definitiva não foi encontrada.
    while(notSolved)
    {
        // Explora o nó de maior prioridade.
        currentNode = unexploredNodes.front();
        pop_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
        unexploredNodes.pop_back();
        //cout << "Next node\nDepth: " << currentNode->depth << " Manhattan: " << currentNode->infos.manhattan_dist() << "\n" << currentNode->infos.toString() << endl;

        // Se a thread encontrou uma solução.
        if (currentNode->infos.manhattan_dist() == 0)
        {
            // Caso mais de uma thread encontre uma solução em tempo similar a melhor solução será escolhida.
            #pragma omp critical
            {
                if(notSolved)
                {
                    notSolved = false;
                    solution = currentNode;
                }
                else if(solution->depth > currentNode->depth)
                {
                    delete solution;
                    solution = currentNode;
                }
                // DEBUG
                cout << "I found it ! Thread: " << myID << endl << currentNode->infos.toString() << endl;
            }
        }
        else
        {
            // Realizar todos os movimentos possíveis no zero.
            for (i = 0; i < 4; i++)
            {
                if (currentNode->movement != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
                {
                    unexploredNodes.push_back(new node(currentNode, movements[i]));
                    push_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
                }
            }
            delete currentNode;
        }
    }

    // DEBUG
    // Limpa memória - Corrigir segfault.
    /*
    for(i = unexploredNodes.size() - 1; i > - 1; i++)
    {
        delete unexploredNodes[i];
    }*/
}
    cout << "Resultado:\tProfundidade:" << solution->depth << "\n" << solution->infos.toString() << endl;

    // Limpando memória alocada.
    delete solution;
}

bool BTtree::node::priorityCalculator::operator() (node *lhs, node *rhs) const
{
    return (lhs->infos.manhattan_dist() + lhs->depth/lhs->infos.getTam()) > (rhs->infos.manhattan_dist() + rhs->depth/lhs->infos.getTam());
}

BTtree::~BTtree()
{
    delete rootNode;
}
