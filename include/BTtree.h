#pragma once

#include <iostream>
#include <queue>
#include <memory>

#include <puzzle.h>

using namespace std;

class BTtree {
public:

	class node
	{
	public:
		shared_ptr<node> father;
		puzzle infos;
		enum puzzle::zero_movement movement;
		int depth;

		node(const puzzle &copy);

		node(shared_ptr<node> lastNode, enum puzzle::zero_movement thisMovement);

		class priorityCalculator
		{
		public:
			bool operator() (shared_ptr<BTtree::node> lhs, shared_ptr<BTtree::node> rhs) const;
		};
	};

	void startDeathRide();

	BTtree(puzzle &original);

private:
	shared_ptr<node> rootNode, currentNode;
	priority_queue<shared_ptr<node>, vector<shared_ptr<node>>, node::priorityCalculator> unexploredNodes;
};
