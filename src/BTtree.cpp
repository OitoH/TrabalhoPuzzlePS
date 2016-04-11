#include <BTtree.h>

BTtree::node::node(const puzzle &copy)
: infos(copy)
, father(NULL)
, movement(puzzle::ZERO_NONE)
, depth(0)
{}

BTtree::node::node(shared_ptr<node> lastNode, enum puzzle::zero_movement thisMovement)
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
    priority_queue<shared_ptr<node>, vector<shared_ptr<node>>, node::priorityCalculator> unexploredNodes;
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
				unexploredNodes.push(shared_ptr<node>(new node(currentNode, it)));

		// Explora o nó de maior prioridade.
		currentNode = unexploredNodes.top();
		unexploredNodes.pop();
        cout << "Next node\nDepth: " << currentNode->depth << "\n" << currentNode->infos.toString() << endl;
	}
	cout << "Resultado:\n" << currentNode->infos.toString() << endl;
}

bool BTtree::node::priorityCalculator::operator() (shared_ptr<BTtree::node> lhs, shared_ptr<BTtree::node> rhs) const
{
    return (lhs->infos.manhattan_dist() + lhs->depth / 2) >= (rhs->infos.manhattan_dist() + rhs->depth / 2);
}
