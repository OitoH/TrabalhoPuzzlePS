#include "../include/BTtree.h"

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

BTtree::BTtree(puzzle &original)
: rootNode(new node(original))
, currentNode(NULL)
{}

void BTtree::startDeathRide()
{
    priority_queue<node *, vector<node *>, node::priorityCalculator> unexploredNodes;
	vector<enum puzzle::zero_movement> movements{
		puzzle::ZERO_UP,
		puzzle::ZERO_DOWN,
		puzzle::ZERO_LEFT,
		puzzle::ZERO_RIGHT
	};
	currentNode = rootNode;
	while(currentNode->infos.manhattan_dist() != 0)
    {
		// Realizar todos os movimentos possíveis no zero.
		for (auto it: movements)
			if (currentNode->movement != puzzle::oppositeMovement(it) && currentNode->infos.isMoveValid(it))
                unexploredNodes.push(new node(currentNode, it));

        delete currentNode;
		// Explora o nó de maior prioridade.
		currentNode = unexploredNodes.top();
		unexploredNodes.pop();
        //cout << "Next node\nDepth: " << currentNode->depth << " Manhattan: " << currentNode->infos.manhattan_dist() << "\n" << currentNode->infos.toString() << endl;
	}
	cout << "Resultado:\n" << currentNode->infos.toString() << endl;
    delete currentNode;
    while(!unexploredNodes.empty())
    {
        currentNode = unexploredNodes.top();
        unexploredNodes.pop();
        delete currentNode;
    }
}

bool BTtree::node::priorityCalculator::operator() (node *lhs, node *rhs) const
{
    return (lhs->infos.manhattan_dist() + lhs->depth/lhs->infos.getTam()) > (rhs->infos.manhattan_dist() + rhs->depth/lhs->infos.getTam());
}

BTtree::~BTtree()
{
    // TODO: Remvoer ponteiros.
}
