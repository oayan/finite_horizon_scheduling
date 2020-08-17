/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef EDGE_H
#define EDGE_H

class Edge
{
public:
    Edge(const double prob, const unsigned previousID, const unsigned nextID, const int action);

    double p;           // Transition probability of the edge

    int action;         // Action taken before the edge

    unsigned prevID;    // Node id of the previous node (:= Current state)

    unsigned nextID;    // Node id of the next node (:= Next state)

    void print_edge();

};

#endif // EDGE_H
