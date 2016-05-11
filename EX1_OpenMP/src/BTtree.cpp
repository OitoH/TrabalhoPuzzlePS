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

        // Uma thread qualquer gera os nós iniciais para todas as threads.
        #pragma omp single
        {
            globalNodes.push_back(new node(rootNode->infos));

            // Enquanto não existir ao menos 1 nó para cada thread ou não for possível nós suficientes para todas.
            do
            {
                currentNode = globalNodes.front();
                globalNodes.pop_front();

                if(currentNode->infos.manhattan_dist() == 0)
                {
                    notSolved = false;
                    solution = currentNode;
                }
                else
                { // Realizar todos os movimentos possíveis no zero.
                    for (i = 0; i < 4; i++)
                    {
                        if (currentNode->movement != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
                        {
                            globalNodes.push_back(new node(currentNode, movements[i]));
                        }
                    }
                    delete currentNode;
                }
            } while(notSolved && globalNodes.size() < threadsNum);
        }

        if (notSolved)
        {
            // Cada thread preenche a sua lista de prioridade.
            for(i = globalNodes.size() - 1 - myID; i > -1; i -= threadsNum)
                unexploredNodes.push_back(globalNodes[i]);

            #pragma omp barrier // Garantir que todas as threads já formaram suas listas.
            #pragma omp single
            globalNodes.clear();

            if(!unexploredNodes.empty())
            {
                make_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());

                // Inicia exploração.
                // Enquanto uma solução definitiva não foi encontrada.
                while(notSolved)
                {
                    // Explora o nó de maior prioridade.
                    currentNode = unexploredNodes.front();
                    pop_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());

                    unexploredNodes.pop_back();

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
                                solution = currentNode;
                            }
                            else
                            {
                                delete currentNode;
                            }
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

                // Liberando memória.
                while(!unexploredNodes.empty())
                {
                    delete unexploredNodes.back();
                    unexploredNodes.pop_back();
                }
            }
        }

        #pragma omp barrier
    }

    cout << "Resultado:\tProfundidade:" << solution->depth << "\n" << solution->infos.toString() << endl;

    // Liberando memória alocada.
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
