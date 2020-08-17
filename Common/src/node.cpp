#include "node.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;


Node::Node(const int &_h, const int* const _state, const int &_N)
    : h(_h), N(_N)
{
    state = new int [3 * N];                        // Allocate memory for state vector
    memcpy(state, _state, 3 * N * sizeof(*state));  // Copy the content of initialization array
    assert(state != _state);
    outgoingEdges.clear();
    incomingEdges.clear();
    feasibleActionsToEdgesMap.clear();
    id = -1;
    cost = 0.0;
    optimalAction = -1;
}

Node::~Node()
{
    for (std::vector<Edge*>::iterator it = outgoingEdges.begin(); it != outgoingEdges.end(); it++)
    {
        delete *it;
    }
    outgoingEdges.clear();

    if (state != NULL)
    {
        delete[] state;
        state = NULL;
    }
}

std::vector<Edge> Node::get_outgoing_edges_for(const int action)
{
    std::vector<Edge> eVec;
    std::vector<std::pair<int, std::vector<unsigned>>>::iterator aIt;

    eVec.clear();

    for(aIt = feasibleActionsToEdgesMap.begin(); aIt != feasibleActionsToEdgesMap.end(); aIt++)
    {
        if(action == (*aIt).first)
        {
            for(unsigned i = 0; i < (*aIt).second.size(); i++)
            {
                unsigned eId = (*aIt).second.at(i);
                eVec.push_back(get_edge(eId));
            }
        }
    }
    return eVec;
}

Edge Node::get_edge(unsigned edgeId)
{
    return *(outgoingEdges.at(edgeId));
}

void Node::getState(int* const _state) const
{
    memcpy(_state, state, 3 * N * sizeof(*state));  // Copy the content of own state to input array
}

int Node::getLatestUtilizedGen(int i) const
{

    //    for(int j = 0; j <  3* N ; j++)
    //        int buf = state[j];
    return state[2 * N + i];
}


unsigned Node::add_outgoing_edge(const double prob, const unsigned nextID, const int action)
{
    // Add edge to the vector
    outgoingEdges.push_back(new Edge(prob, id, nextID, action));

    // Return edge position (=edgeID) in the list
    return outgoingEdges.size() - 1;
}

void Node::add_action_and_edges(const int a, const unsigned *edgeIDs, const unsigned& eCnt)
{
    std::vector<unsigned> vec;
    vec.reserve(eCnt);
    for (unsigned i = 0; i < eCnt; i ++)
    {
        vec.push_back(edgeIDs[i]);
    }
    assert(eCnt == vec.size());
    feasibleActionsToEdgesMap.push_back(make_pair(a, vec));
}
