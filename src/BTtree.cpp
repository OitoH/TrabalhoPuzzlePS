#include <BTtree.h>

BTtree::node::node(puzzle &copy)
: infos(copy)
, father(NULL)
, movement(ZERO_NONE)
, depth(0)
{}

BTtree::node::node(shared_ptr<node> lastNode, enum zero_movement thisMovement)
	: infos(lastNode->infos)
	, father(lastNode)
	, movement(thisMovement)
{
	depth = father->depth + 1;
	infos.move_zero(movement);
}

BTtree::BTtree(puzzle &original)
: rootNode(new node(original))
, currentNode(NULL)
{}

void BTtree::startDeathRide()
{
	currentNode = rootNode;
	while(currentNode->infos.manhattan_dist() != 0)
	{
		// Realizar todos os movimentos possíveis no zero.
		for (enum zero_movement it = ZERO_UP; it < ZERO_NONE; it + 1)
			if (currentNode->infos.isMoveValid(it))
				unexploredNodes.push(shared_ptr<node>(new node(currentNode, it)));

		// Explora o nó de maior prioridade.
		currentNode = unexploredNodes.top();
		unexploredNodes.pop();
	}
	cout << "Resultado:\n" << currentNode->infos.toString() << endl;
}

bool BTtree::node::priorityCalculator::operator() (shared_ptr<BTtree::node> lhs, shared_ptr<BTtree::node> rhs) const
{
	return (lhs->infos.manhattan_dist() + lhs->depth) < (rhs->infos.manhattan_dist() + rhs->depth);
}
