#include "../include/BTtree.h"
#include <algorithm>

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
, currentNode(NULL)
{}

void BTtree::startDeathRide()
{
    int i;
    vector<node *> unexploredNodes;
    enum puzzle::zero_movement movements[4] = {
		puzzle::ZERO_UP,
		puzzle::ZERO_DOWN,
		puzzle::ZERO_LEFT,
		puzzle::ZERO_RIGHT
	};

    currentNode = new node(rootNode->infos);
	while(currentNode->infos.manhattan_dist() != 0)
    {
		// Realizar todos os movimentos possíveis no zero.
        for (i = 0; i < 4; i++)
            if (currentNode->movement != puzzle::oppositeMovement(movements[i]) && currentNode->infos.isMoveValid(movements[i]))
            {
                unexploredNodes.push_back(new node(currentNode, movements[i]));
                push_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
            }

        delete currentNode;

		// Explora o nó de maior prioridade.
        currentNode = unexploredNodes.front();
        pop_heap(unexploredNodes.begin(), unexploredNodes.end(), node::priorityCalculator());
        unexploredNodes.pop_back();
        //cout << "Next node\nDepth: " << currentNode->depth << " Manhattan: " << currentNode->infos.manhattan_dist() << "\n" << currentNode->infos.toString() << endl;
	}
	cout << "Resultado:\n" << currentNode->infos.toString() << endl;

    // Limpando memória alocada.
    delete currentNode;
    for (i = unexploredNodes.size() - 1; i > -1; i--)
    {
        currentNode = unexploredNodes[i];
        delete currentNode;
    }
}

bool BTtree::node::priorityCalculator::operator() (node *lhs, node *rhs) const
{
    return (lhs->infos.manhattan_dist() + lhs->depth/lhs->infos.getTam()) > (rhs->infos.manhattan_dist() + rhs->depth/lhs->infos.getTam());
}

BTtree::~BTtree()
{
    delete rootNode;
}
