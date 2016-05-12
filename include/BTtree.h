#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <deque>

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
    BTtree();
    ~BTtree();

    // Manipulation
    bool generateInitialNodes(unsigned mpi_npes); // returns 'true' if found a solution
    bool startDeathRide(bool *keepRunning);

    deque<node*>* generatedNodes();
    node *getSolution();
    int getSolutionDepth();

private:
    // Pointer to root node
    node *rootNode;

    // Nodes
    deque<node*> globalNodes;
    node *solution;
    int solutionDepth;

};
