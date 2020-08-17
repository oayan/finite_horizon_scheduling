/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef TREE_H
#define TREE_H

#include "node.h"


class Tree
{
public:
    /**
     * @brief Tree Class constructor
     * @param root Root node given from outside. Current state of the problem
     * @param mts Maximum number of nodes allowed
     * @param H (Given) Finite horizon. H=1 for a greedy implementation. A higher H indicates a more far-sighted tree.
     */
    Tree(Node &root, unsigned _mts, unsigned _H, unsigned _N,
         const int _t, const int * const _samplingOffsets, const int * const _samplingPeriods,
         const double * const _lossProbabilities);

    /**
     * @brief ~Tree Class destructor.
     */
    ~Tree();

    /**
     * @brief add_node Add a node to the root tree
     * @param h Level of the node within the tree
     * @param state Node state, e.g. AoI
     * @return Assigned unique node ID within the tree
     */
    unsigned add_node(const int &h, const int* const state);

    /**
     * @brief construct_root_tree Construct the root tree starting from the root node.
     */
    void construct_root_tree();

    /**
     * @brief get_size Returns tree size, i.e., number of nodes within the root tree
     */
    unsigned get_size() { return nodes.size(); }

    /**
     * @brief initialize_costs Iterates backwards from leaf nodes towards root. Calculates costs of each node with Bellman-Equation.
     */
    void initialize_costs(double **costMap, size_t rows, size_t cols);

    int getOptimalAction();

    //    void print_nodes();

private:
    unsigned maxTreeSize;                           // Maximum allowed number of nodes.

    unsigned H;                                     // Finite horizon of the tree

    unsigned N;                                     // Number of users

    int* actionSpace;                               // Action space, i.e., which loop can be scheduled, given current state

    int* samplingOffsets;

    int* samplingPeriods;

    double* lossProbabilities;

    int t;

    bool optimalReady;

    std::vector<Node*> nodes;                       // Stores pointers to nodes

    std::vector<std::pair<int, int>> levels_table;  // Stores starting and ending node IDs for each level of the tree

    /**
     * @brief construct_next_level Given current level 'h' nodes, constructs next level 'h+1' of the root tree
     * @param h Current level
     * @param curLevelStart Starting node of the current level
     * @param curLevelEnd Ending node of the current level
     * @return Returns starting and ending indices of next level nodes
     */
    std::pair<int, int> construct_next_level(const unsigned& h, const int& curLevelStart, const int& curLevelEnd);

    int get_feasible_actions(const int * const x_h, int * const faa);

    void get_next_state_of_loop(const int &h, const int * const states, int *const nextStates, const int &i, const bool &loss);
};

#endif // TREE_H
