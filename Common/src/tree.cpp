#include "tree.h"

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>
#include <map>
#include <assert.h>     /* assert */

using namespace std;

static bool wFlag = false;


Tree::Tree(Node& root, unsigned _mts, unsigned _H, unsigned _N, const int _t,
           const int * const _samplingOffsets, const int * const _samplingPeriods,
           const double* const _lossProbabilities)
    : maxTreeSize(_mts), H(_H), N(_N), t(_t)
{
    nodes.clear();
    int rootState[3 * N];
    root.getState(rootState);
    add_node(root.h, rootState);
    levels_table.clear();
    levels_table.reserve(H);

    actionSpace = new int [N];
    samplingOffsets = new int[N];
    samplingPeriods = new int[N];
    lossProbabilities = new double[N];

    for (unsigned i = 0; i < N; i++)
        actionSpace[i] = i;

    memcpy(samplingOffsets, _samplingOffsets, N * sizeof(*samplingOffsets));

    memcpy(samplingPeriods, _samplingPeriods, N * sizeof(*samplingPeriods));

    memcpy(lossProbabilities, _lossProbabilities, N * sizeof (*lossProbabilities));

    optimalReady = false;
}

Tree::~Tree()
{
    for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        delete *it;

    nodes.clear();
    if(actionSpace !=NULL){
        delete[] actionSpace;
        actionSpace = NULL;
    }
    if(samplingOffsets != NULL)
    {
        delete[] samplingOffsets;
        samplingOffsets = NULL;
    }
    if(samplingPeriods != NULL)
    {
        delete[] samplingPeriods;
        samplingPeriods = NULL;
    }
}

void Tree::get_next_state_of_loop(const int& h, const int* const states, int * const nextStates, const int& i, const bool& loss)
{
    int nextSlotTime = t + h + 1;

    // Get the generation time of the latest sampled packet at the nextSlotTime
    int nextGenTime = nextSlotTime - ((nextSlotTime - samplingOffsets[i]) % samplingPeriods[i]);

    if(!loss)
    {   // If successful transmission:

        // Copy current sampled packet into next states queued packet
        nextStates[i] = states[i];
        nextStates[N + i] = states[i];
        nextStates[2 * N + i] = states[2 * N + i];
    }
    else
    {
        // If failed transmission:

        // Copy current sampled packet into next states queued packet
        nextStates[i] = states[i];
        nextStates[N + i] = states[N + i];
        nextStates[2 * N + i] = states[2 * N + i];
    }

    if(nextGenTime == nextSlotTime)
    {   // If next time slot will be a sampling time:

        // Update generation time of the sampled packet
        nextStates[i] = nextSlotTime;

        // Update generation time of the utilized packet
        nextStates[2 * N + i] = nextStates[N + i];
    }

    if (floor((nextSlotTime - nextStates[2 * N + i]) / samplingPeriods[i]) >= 100 && !wFlag)
    {
        wFlag = true;
        std::wcout << "Age of 100 reached";
    }
    // assert();
}

int Tree::get_feasible_actions(const int* const x_h, int* const faa)
{
    int faCnt = 0;
    // Initialize all actions as 'infeasible'
    memset(faa, 0, N*sizeof(*faa));

    /* Here comes the feasibility check*/
    for (unsigned i = 0; i < N; i++)
    {
        // Feasible, if loop has a new packet to transmit
        if(x_h[i] > x_h[N + i])
        {
            faa[i] = 1;
            faCnt++;
        }
    }

    return faCnt;
}


unsigned Tree::add_node(const int& h, const int* const state)
{
    // Allocate dynamic memory to node.
    Node* n = new Node(h, state, this->N);

    // Nodes get unique IDs with increasing order. (Note that we never remove nodes, therefore unique)
    n->set_id(this->nodes.size());

    nodes.push_back(n);

    return n->get_id();
}


//void Tree::print_nodes()
//{
//    for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); it++)
//    {
//        cout << "h: "<<  (*it)->h << " state: " << (*it)->state << " cost: " << (*it)->get_cost()
//             << " optimal action: " << (*it)->get_optimal_action() << endl ;
//    }
//}


std::pair<int, int> Tree::construct_next_level(const unsigned &h, const int& curLevelStart, const int& curLevelEnd)
{
    // State --> Node ID mapping to avoid duplicating same state nodes
    std::map<const int, const unsigned> nextLevelMap;
    std::map<const int, const unsigned>::iterator mapIt;

    // Next level starts after this level's end index
    int nextLevelStart = curLevelEnd + 1;

    // This is going to be incremented inside the for loop
    int nextLevelEnd = nextLevelStart;

    int state[3 * N];
    int successState[3 * N];
    int failedState[3 * N];
    int failedId;

    // Iterate through current level nodes to define next level nodes
    for(int curNode = curLevelStart; curNode <= curLevelEnd; curNode++)
    {
        // Read (copy) node state from node object into 'nState' array
        nodes.at(curNode)->getState(state);

        //        for(int x=0; x < 3 *N; x++)
        //            qDebug() << state[x];

        // Construct 'no-update' next state
        for(unsigned i = 0; i < N; i++)
            get_next_state_of_loop(h, state, failedState, i, true);

        //        for(int x=0; x < 3 *N; x++)
        //            qDebug() << failedState[x];

        // Add to next level nodes list
        failedId = add_node(h+1, failedState);

        int feasibleActions[N];

        // Feasible action given a state is given by the problem definition & constraints.
        // Read feasible actions given the state of the node
        int faCnt = get_feasible_actions(state, feasibleActions);

        // For each feasible action --> a successful and a failed transmission edge
        unsigned edgeIDs[2 * faCnt];

        if(faCnt == 0)
        {
            // Add the outgoing edge to current node's list. Returns the edgeID.
            unsigned edgeID = nodes.at(curNode)->add_outgoing_edge(1.0, failedId, -1);
            assert(edgeID == 0);
            // Add the only edge with action -1 (no schedule)
            nodes.at(curNode)->add_action_and_edges(-1, &edgeID, 1);
        }
        else
        {
            // Iterate through feasible actions array and calculate possible next states
            unsigned faaCnt = 0;
            for(unsigned i = 0; i < N; i++)
            {
                if(feasibleActions[i])
                {
                    // Add the 'loss transition':
                    edgeIDs[2 * faaCnt] = nodes.at(curNode)->add_outgoing_edge(lossProbabilities[i], failedId, i);
                    assert(edgeIDs[2 * faaCnt] == 2 * faaCnt);

                    // Add the 'success transition':
                    // First get failed state as baseline
                    memcpy(successState, failedState, 3 * N * sizeof(*successState));
                    //                    qDebug() << "Success state";
                    //                    for(int x=0; x < 3 * N; x++)
                    //                        qDebug() << successState[x];

                    // Update the next state of the sucessful loop, keep the rest same
                    get_next_state_of_loop(h, state, successState, i, false);

                    // Add to next level nodes list
                    int successId = add_node(h+1, successState);

                    // Add the outgoing edge to current node's list. Returns the edgeID.
                    edgeIDs[2 * faaCnt + 1] = nodes.at(curNode)->add_outgoing_edge(1.0 - lossProbabilities[i], successId, i);
                    assert(edgeIDs[2 * faaCnt + 1] == 2 * faaCnt + 1);

                    // Add these two outgoing edge IDs as possible transitions after this state-action pair
                    nodes.at(curNode)->add_action_and_edges(i, &(edgeIDs[2 * faaCnt]), 2);

                    faaCnt++;
                }
            }
        }
    }

    // Last node's index is the end of this level
    nextLevelEnd = nodes.back()->get_id();

    return std::make_pair(nextLevelStart, nextLevelEnd);
}

void Tree::construct_root_tree()
{
    // Level 0 is always the root node
    unsigned lev = 0;
    pair<int, int> currentLevel = make_pair(0,0);
    levels_table.push_back(currentLevel);

    while(lev < H)
    {
        currentLevel = construct_next_level(lev, get<0>(currentLevel), get<1>(currentLevel));
        levels_table.push_back(make_pair(get<0>(currentLevel), get<1>(currentLevel)));
        lev++;
    }
}

void Tree::initialize_costs(double **costMap, size_t rows, size_t cols)
{
    static int ctr = 0;
    assert(N = rows);

    int maxAoI = cols;

    int h = H;

    std::vector<int> equalActions;

    //Iterate through levels in reverse order
    while (h >= 0)
    {
        // Iterate through all nodes in the current level of the root tree
        for(vector<Node*>::iterator it = nodes.begin() + levels_table[h].first; it <= nodes.begin() + levels_table[h].second; it++)
        {

            if(h == (int) H)
            {   // If leaf node: Cost is simply terminal costs, e.g. age-penalty
                double cost = 0.0;
                for(unsigned i = 0; i < N; i++)
                {
                    int nom = t + (*it)->h - (*it)->getLatestUtilizedGen(i);
                    int denom = samplingPeriods[i];
                    int AoI_i = min(maxAoI, static_cast<int>(floor(nom / denom)));
                    cost += costMap[i][AoI_i];
                }
                (*it)->set_cost(cost);
            }
            else
            {
                // Invalid action (initialization)
                int optimalAction = -1;

                // Infinite initial cost
                double minimumCost = std::numeric_limits<double>::max();

                // Cost of a state is immediate cost + expected future costs when best action is taken
                std::vector<std::pair<int, std::vector<unsigned>>>::iterator aIt;
                for(aIt = (*it)->feasibleActionsToEdgesMap.begin(); aIt != (*it)->feasibleActionsToEdgesMap.end(); aIt++)
                {
                    const int action = (*aIt).first;
                    std::vector<unsigned> edgeIDs = (*aIt).second;

                    // Immediate cost
                    double cost = 0.0;
                    for(unsigned i = 0; i < N; i++)
                    {
                        int AoI_i = static_cast<int>(floor((t + (*it)->h - (*it)->getLatestUtilizedGen(i)) / samplingPeriods[i]));
                        cost += costMap[i][AoI_i];
                    }

                    // With expected future cost
                    for (std::vector<unsigned>::iterator eIt = edgeIDs.begin(); eIt != edgeIDs.end(); eIt++) {
                        unsigned eID = (*eIt);

                        // Transition probability of edge x Cost of Next State
                        cost += (*it)->get_edge(eID).p * nodes.at((*it)->get_edge(eID).nextID)->get_cost();

                        // Bound already if cost exceeds best cost
                        if (cost > minimumCost)
                            break;
                    }

                    // Update minimum cost and optimal action
                    if (cost < minimumCost)
                    {
                        equalActions.clear();
                        minimumCost = cost;
                        optimalAction = action;
                    }
                    else if(cost == minimumCost)
                    {
                        if(equalActions.empty())
                        {
                            equalActions.push_back(optimalAction);
                            equalActions.push_back(action);
                        }
                        else
                            equalActions.push_back(action);
                    }
                }

                if(!equalActions.empty())
                    optimalAction = equalActions.at(rand() % equalActions.size());
                // Set cost of node to minimum cost and corresponding optimal action to take
                (*it)->set_cost(minimumCost);
                (*it)->set_optimal_action(optimalAction);
            }
        }

        h--;
    }
    optimalReady = true;
}

int Tree::getOptimalAction()
{
    assert(optimalReady == true);

    return nodes.at(0)->get_optimal_action();
}

