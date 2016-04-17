#pragma once

#include <iostream>
#include <memory>

#include "../include/puzzle.h"

using namespace std;

class BTtree {
public:

	class node
	{
    public:
		puzzle infos;
		enum puzzle::zero_movement movement;
		int depth;

		node(const puzzle &copy);

        node(node *lastNode, enum puzzle::zero_movement thisMovement);

		class priorityCalculator
		{
		public:
            bool operator() (node *lhs, node *rhs) const;
		};
	};

	void startDeathRide();

    BTtree(const puzzle &original);

    ~BTtree();

private:
    node *rootNode;
};
