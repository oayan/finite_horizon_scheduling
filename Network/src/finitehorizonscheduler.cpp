#include "finitehorizonscheduler.h"
#include "Common/node.h"
#include "Common/tree.h"

const unsigned maximumTreeSize = 1000; //UNUSED NOW

const int maximumHorizon = 5;

FiniteHorizonScheduler::FiniteHorizonScheduler()
    : Scheduler()
{

    assert(states == NULL);

    costMap = NULL;

    totalNodeCount = 0;

    numNodeCountMeasurements = 0;
}

void FiniteHorizonScheduler::scheduleUL(const int *demandVec, int *allocVec, const int &userCnt)
{
    int totalRemainingDemand = 0;

    int numRemainingResources = ULnumRB;

    assert(numRemainingResources < 2);      // This loop is not intended for that

    totalULlresources += ULnumRB;

    int remainingDemandVec[userCnt];

    for(int i = 0; i < userCnt; i++)
    {
        totalRemainingDemand += demandVec[i];
        remainingDemandVec[i] = demandVec[i];
    }

    //    double highestULUtility = 0;
    int optimalAction = 0;        // Index 'i' of the loop

    // Keep allocating UL resources as long as all of the following hold:
    // 1) there are remaining UL resources left to allocate
    // 2) there is UL demand left to schedule
    while(numRemainingResources > 0 && totalRemainingDemand > 0)
    {
        // For now. Otherwise, check if the rest makes sense
        assert(numRemainingResources == 1);

        // Current networks state as root node
        Node rootNode(0, states, N);

        // qDebug() << "FiniteHorizonScheduler::scheduleUL: AoI = " << floor((t - states[2]) / samplingPeriods[0]);

        // Construct root tree given current network state up to horizon H
        Tree tree(rootNode, maximumTreeSize, maximumHorizon, N, t, samplingOffsets, samplingPeriods, lossProbabilities);
        tree.construct_root_tree();

        // Keep node count stats
        numNodeCountMeasurements++;
        unsigned long nodeCount = tree.get_size();
        assert(nodeCount > 0);
        totalNodeCount += nodeCount;

        // Initialize Costs
        tree.initialize_costs(costMap, N, maxAoI);
        // Get optimal action
        optimalAction =  tree.getOptimalAction();

        // Execute resource allocation
        if(optimalAction >=0)
        {
            // Because userIDs include receivers as well
            // Did not change initial version of the code. This was the fastest fix.
            int allocatedUserId = optimalAction * 2;

            int allocatedResources = std::min(numRemainingResources, remainingDemandVec[allocatedUserId]);
            remainingDemandVec[allocatedUserId] -= allocatedResources;
            allocVec[allocatedUserId] = allocatedResources;
            usedULresources += allocatedResources;
            numRemainingResources -= allocatedResources;
            totalRemainingDemand -= allocatedResources;
        }
        else
        {
            assert(totalRemainingDemand == 0);
            break;
        }
    } // End of while loop
    wastedULresources += numRemainingResources;
    assert(numRemainingResources >= 0);

}

FiniteHorizonScheduler::~FiniteHorizonScheduler()
{
    std::vector<ControlApplication*>::iterator loop_itr;

    for(loop_itr = controlLoops.begin(); loop_itr != controlLoops.end(); loop_itr++)
        delete *loop_itr;

    controlLoops.clear();

    if(states != NULL)
        delete [] states;

    if(samplingOffsets != NULL)
        delete [] samplingOffsets;


    if(costMap != NULL)
    {
        //Free each sub-array
        for(unsigned r = 0; r < N; ++r)
            delete[] costMap[r];

        //Free the array of pointers
        delete[] costMap;
        costMap = NULL;
    }
}

ControlApplication* FiniteHorizonScheduler::getAppByTxId(int txID)
{
    std::vector<ControlApplication*>::iterator it;
    // Collect resource requests
    for(it = controlLoops.begin(); it != controlLoops.end(); it++)
    {
        if((*it)->getTransmitterID() == txID)
            return (*it);
    }

    return NULL;
}

void FiniteHorizonScheduler::updateULUtility(ControlApplication* const app)
{

}

void FiniteHorizonScheduler::timePass_1ms(int t)
{
    this->t = t;
    std::vector<ControlApplication*>::iterator loop_itr;
    int i = 0;

    for(loop_itr = controlLoops.begin(); loop_itr != controlLoops.end(); loop_itr++)
    {
        ControlApplication* pApp = *loop_itr;

        if(pApp->timePass_1ms(t))
        {   // If, application has just been sampled, i.e., AoI increased by one
            pApp->mimicSampling();

            assert(t == t - ((t - samplingOffsets[i]) % samplingPeriods[i]));
            // First N values: generation time of the latest sampled packet

            states[i] = t;
            // qDebug() << "FHS::timepass(): Sensor should have just sampled pkt with generated timestamp: t_s =" << t;


            // Second N values: generation time of the latest queued packet
            // Third N values: generation time of the latest utilized packet

            // qDebug() << "FHS::timepass(): Controller should have just read pkt with generated timestamp: t_s =" << states[N + i];

            states[2 * N + i] = states[N + i];

            updateULUtility(pApp);
        }
        i++;
    }
}

void FiniteHorizonScheduler::initStates()
{
    N = controlLoops.size();

    states = new int [3 * N];

    samplingOffsets = new int [N];

    samplingPeriods = new int [N];

    lossProbabilities = new double [N];

    for (unsigned i = 0; i < N; ++i)
    {
        int t_s = controlLoops.at(i)->sampling_period_ms;
        samplingPeriods[i] = t_s;

        int t_o = controlLoops.at(i)->samplingOffset_ms;
        samplingOffsets[i] = t_o;

        // First N values: generation time of the latest sampled packet
        // Second N values: generation time of the latest queued packet
        // Third N values: generation time of the latest utilized packet
        states[i] = 0;
        states[N + i] = states[i] - samplingPeriods[i];
        states[2 * N + i] = states[i] - samplingPeriods[i];
    }
    // Generate age-to-cost map for all loops up to 'maxAoI' age
    initCostMap();
}

void FiniteHorizonScheduler::initLossProbabilties()
{
    for (unsigned i = 0; i < N; ++i)
        lossProbabilities[i] = controlLoops.at(i)->getTransmitter()->lossProbability;
}

void FiniteHorizonScheduler::initCostMap()
{
    // Call this at least once at the beginning of the simulation
    assert(costMap == NULL);

    unsigned rowCount = controlLoops.size();
    unsigned colCount = maxAoI;

    costMap = new double*[rowCount];
    for(unsigned c = 0; c < rowCount; ++c)
        costMap[c] = new double[colCount];

    std::vector<ControlApplication*>::iterator it;

    int r = 0;

    for(it = controlLoops.begin(); it != controlLoops.end(); it++)
    {
        ControlApplication* pApp = *it;

        for(unsigned c = 0; c < maxAoI; c++)
        {
            double ageCost = ((pApp->getApower(c - 1).transpose() * pApp->getApower(c - 1)) * pApp->W).trace();

            if(c == 0)
                costMap[r][c] = 0;
            else
                costMap[r][c] = costMap[r][c - 1] + ageCost;
        }
        r++;
    }
}

void FiniteHorizonScheduler::update(const packet& pkt)
{
    std::vector<ControlApplication*>::iterator it;
    // Collect resource requests
    int i = 0;
    for(it = controlLoops.begin(); it != controlLoops.end(); it++)
    {
        if((*it)->getReceiverID() == pkt.destinationID)
        {
            (*it)->l = pkt.timeStamp;
            // First N values: generation time of the latest sampled packet
            // Second N values: generation time of the latest queued packet
            // Third N values: generation time of the latest utilized packet
            states[N + i] = states[i];
            return;
        }
        i++;
    }
}
