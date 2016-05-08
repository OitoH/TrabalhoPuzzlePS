#include "../include/BTtree.h"
#include <algorithm>
#include <deque>
#include <omp.h>
#include <set>
#include <map>
#include <cstring>

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

    class compareKeyAsPointer {
    public:
        bool operator()(puzzle::Key* a, puzzle::Key* b) {return a->operator <(*b);}
    };

    bool notSolved = true;
    set<puzzle::Key*, compareKeyAsPointer> exploredNodes;
    deque<node *> globalNodes;
    set<node *, node::priorityCalculator> unexploredNodes;
    set<node *, node::priorityCalculator>::iterator pop_front;
    node *newNode, *currentNode, *solution = nullptr;

    #pragma omp parallel private(i, threadsNum, myID, unexploredNodes, exploredNodes, newNode, currentNode), shared(globalNodes, notSolved, solution)
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
            for(i = globalNodes.size() - 1; i > -1; --i)
            {
                if(i % threadsNum == myID)
                    unexploredNodes.insert(globalNodes[i]);
                else
                    exploredNodes.insert(globalNodes[i]->infos.getKey());
            }

            #pragma omp barrier // Garantir que todas as threads já formaram suas listas.
            #pragma omp single
                globalNodes.clear();

            if(!unexploredNodes.empty())
            {
                // Inicia exploração.
                // Enquanto uma solução definitiva não foi encontrada.
                while(notSolved)
                {
                    // Explora o nó de maior prioridade.
                    pop_front = unexploredNodes.begin();
                    currentNode = *pop_front;
                    unexploredNodes.erase(currentNode);

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
                                newNode = new node(currentNode, movements[i]);
                                if (exploredNodes.find(newNode->infos.getKey(true)) != exploredNodes.end()
                                   || unexploredNodes.insert(newNode).second == false)
                                    delete newNode;
                            }
                        }
                        exploredNodes.insert(currentNode->infos.getKey());
                        delete currentNode;
                    }
                }

                // Liberando memória.
                for(auto it = unexploredNodes.begin(); it != unexploredNodes.end(); ++it)
                    delete *it;
                unexploredNodes.clear();

                for(auto it = exploredNodes.begin(); it != exploredNodes.end(); ++it)
                    delete *it;
                exploredNodes.clear();
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
    int lhs_value = lhs->infos.manhattan_dist() + lhs->depth/lhs->infos.getTam(),
        rhs_value = rhs->infos.manhattan_dist() + rhs->depth/lhs->infos.getTam();
    if (lhs_value == rhs_value)
        return lhs->infos.getKey(true)->operator <(*rhs->infos.getKey(true));
    else
        return rhs_value < lhs_value;
}

BTtree::~BTtree()
{
    delete rootNode;
}
