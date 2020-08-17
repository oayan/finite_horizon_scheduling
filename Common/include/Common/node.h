/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef NODE_H
#define NODE_H
#include <vector>
#include "edge.h"

class Node
{
public:
    /**
     * @brief Node Node class constructor
     * @param _h Level of the node within the root tree
     * @param _state State of the node, i.e., AoI vector
     */
    Node(const int& _h, const int * const _state, const int& N);

    /**
     * ~Node Node class destructor
     */
    ~Node();

    int h;          // Level of the node within the root tree

    /**
     * @brief add_outgoing_edge Each node keeps outgoing edges from itself in a vector o Edge pointers.
     * This method allocates dynamic memory for a new edge, and adds its address to the list.
     * @param prob Transition probability of the edge
     * @param nextID Unique Id of the next node (: "Next State") within the node tree. Given by the Tree objec.
     * @param action Taken action for this edge
     */
    unsigned add_outgoing_edge(const double prob, const unsigned nextID, const int action);

    /**
     * @brief add_action_and_edges Creates an 'action --> possible outgoing edges' map.
     * Used to obtain the expected cost for each feasible action
     * @param a Action
     * @param edgeIDs Array storing the positions of the possible outgoing edges in 'this->outgoingEdges' list
     * @param eCnt Number of possible edges
     */
    void add_action_and_edges(const int a, const unsigned* edgeIDs, const unsigned &eCnt);

    /**
     * @brief get_outgoing_edges_for Returns a vector of edges that are possible if an action is taken.
     * @param action Action to take
     * @return Returns a vector of edges
     */
    std::vector<Edge> get_outgoing_edges_for(const int action);

    /**
     * @brief get_edge Get edge with ID
     * @param edgeId Edge's position in 'outgoingEdges' list
     * @return Returns a copy of the edge
     */
    Edge get_edge(unsigned edgeId);

    void set_id(const int id) { this->id = id;}

    int get_id() {return id;}

    void set_cost(double c) { cost = c; }

    double get_cost() { return cost; }

    int get_optimal_action() { return optimalAction; }

    void set_optimal_action(int a) { optimalAction = a; }

    std::vector<std::pair<int, std::vector<unsigned>>> feasibleActionsToEdgesMap;

    std::vector<Edge*> outgoingEdges;

    std::vector<Edge> incomingEdges;

    void getState(int* const _state) const;

    int getLatestUtilizedGen(int i) const;

private:
    int* state;

    unsigned N;

    int id;         // Unique ID of the node within the root tree

    double cost;    // Optimal cost of the node (book notation: J*)

    int optimalAction;  // Optimal action to take minimizing expected future costs (book notation: u*)
};

#endif // NODE_H
