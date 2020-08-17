#include "edge.h"
#include <iostream>

using namespace std;
Edge::Edge(const double prob, const unsigned previousID, const unsigned nextID, const int action)
{
    this->p = prob;
    this->prevID = previousID;
    this->nextID = nextID;
    this->action = action;
}

void Edge::print_edge()
{
    cout << "(" << prevID << ") --> (" << nextID << ")    p: " << p << ", a: " << action << endl;
}
