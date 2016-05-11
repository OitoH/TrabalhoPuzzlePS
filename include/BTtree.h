#pragma once

#include <iostream>
#include <memory>

#include "../include/puzzle.h"

using namespace std;

class BTtree {
public:

    // Node class
    class node {
    public:
        // Node infos
        puzzle infos;
        enum puzzle::zero_movement moviment;
		int depth;

        // Constructors
		node(const puzzle &copy);
        node(node *lastNode, enum puzzle::zero_movement thisMovement);

        // Priority calculator for queue
        class priorityCalculator {
		public:
            bool operator() (node *lhs, node *rhs) const;
		};
	};

    // Constructors
    BTtree(const puzzle &original);
    ~BTtree();

    // Start
	void startDeathRide();

private:
    // Pointer to root node
    node *rootNode;
};
