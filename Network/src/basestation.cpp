#include "basestation.h"
#include "NetworkBase/gateway.h"
#include <math.h>
#include "finitehorizonscheduler.h"
#include "aoischeduler.h"
#include "networkconfig.h"
#include <chrono>


BaseStation::BaseStation(SchedulerTypes st)
    : schType(st)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    uniDistribution = std::uniform_real_distribution<double>(0, 1);

    switch(schType)
    {

    case AOI:
    {
        scheduler = new AOIScheduler();
        break;
    }

    case FINITE_HORIZON_SCHEDULER:
    {
        scheduler = new FiniteHorizonScheduler();
        break;
    }

    default:
        break;
    }

    for(int i = BS_MAX_USERS; i >= 0; i--)
        availableNetworkIDs.push_back(i);
}

int BaseStation::addUser(MobileTerminal * const mt)
{
    if(availableNetworkIDs.empty())
    {
        std::cerr << "Maximum amount of users reached!" << std::endl;
        return 0;
    }
    int id = availableNetworkIDs.back();
    mt->setID(id);
    availableNetworkIDs.pop_back();
    users.push_back(mt);
    return id;
}

void BaseStation::init_scheduler()
{

    switch(schType)
    {

    case AOI:
    {
        AOIScheduler* AoIscheduler = dynamic_cast<AOIScheduler *>(scheduler);
        //        AoIscheduler->initStates();        // Does Nothing
        //        AoIscheduler->initLossProbabilties();
        break;
    }

    case FINITE_HORIZON_SCHEDULER:
    {
        FiniteHorizonScheduler* fhScheduler = dynamic_cast<FiniteHorizonScheduler *>(scheduler);
        fhScheduler->initStates();
        fhScheduler->initLossProbabilties();
        break;
    }

    default:
        break;
    }
}


MobileTerminal *BaseStation::getUserWithID(const int & userID)
{
    std::vector<MobileTerminal*>::iterator it;

    // Collect resource requests
    for(it = users.begin(); it != users.end(); it++)
    {
        if ((*it)->getID() == userID)
            return (*it);
    }
    return 0;
}


BaseStation::~BaseStation()
{

    users.clear();

    switch(schType)
    {

    case AOI:
    {
        AOIScheduler* AoIscheduler = dynamic_cast<AOIScheduler *>(scheduler);
        if(AoIscheduler != NULL)
        {
            delete AoIscheduler;
            AoIscheduler = NULL;

        }
        break;
    }

    case FINITE_HORIZON_SCHEDULER:
    {
        FiniteHorizonScheduler* fhScheduler = dynamic_cast<FiniteHorizonScheduler *>(scheduler);

        if(fhScheduler != NULL)
        {
            delete fhScheduler;
            fhScheduler = NULL;
        }
        break;
    }

    default:
        break;
    }
}


void BaseStation::update(int t)
{
    switch(schType)
    {

    case AOI:
    {
        AOIScheduler * aoiScheduler = dynamic_cast<AOIScheduler *>(scheduler);
        aoiScheduler->timePass_1ms(t);
        break;
    }
    case FINITE_HORIZON_SCHEDULER:
    {
        FiniteHorizonScheduler * fhScheduler = dynamic_cast<FiniteHorizonScheduler *>(scheduler);
        fhScheduler->timePass_1ms(t);
        break;
    }
    default:
        break;
    }

    int userCnt = users.size();
    std::vector<MobileTerminal*>::iterator it;
    // +1 because network IDs start from 1 (Outdated)
    //int ULdemandVec[userCnt + 1] = { }; This was how it was done before
    int ULdemandVec[userCnt];
    int ULallocVec[userCnt];
    memset(ULdemandVec, 0, (userCnt) * sizeof(int));
    memset(ULallocVec, 0, (userCnt) * sizeof(int));

    int i = 0;

    // Collect resource requests
    for(it = users.begin(); it != users.end(); it++)
    {
        if((*it)->timePass_1ms() && (*it)->transmitter)
        {
            scheduler->lossProbabilities[i] = (*it)->lossProbability;
        }

        if ((*it)->isRequestingUplink())
        {
            ULdemandVec[(*it)->getID()] = 1;
            assert(scheduler->states[scheduler->N + i] < scheduler->states[i]);
        }

        if((*it)->transmitter)
            i++;
    }

    bool txSuccess = false;
    packet txedPkt;

    // Run scheduling algorithm to determine allocation vector
    switch(schType)
    {

    case AOI:
    {
        AOIScheduler * aoiScheduler = dynamic_cast<AOIScheduler *>(scheduler);
        aoiScheduler->scheduleUL(ULdemandVec, ULallocVec, userCnt);

        i = 0;
        // Execute UL scheduling & transmission
        for(it = users.begin(); it != users.end(); it++)
        {
            int userID = (*it)->getID();

            if (ULallocVec[userID] > 0 && ULdemandVec[userID] > 0)
            {
                if(uniDistribution(generator) > (*it)->lossProbability)
                {
                    txedPkt = (*it)->transmit();
                    txSuccess = true;
                    break;
                }
            }

            // Count loop
            if((*it)->transmitter)
                i++;

        }

        // Reception at the end of the slot
        if(txSuccess)
        {
            getUserWithID(txedPkt.destinationID)->receive(txedPkt);
            aoiScheduler->update(txedPkt);
        }
        break;
    }

    case FINITE_HORIZON_SCHEDULER:
    {
        FiniteHorizonScheduler* fhScheduler = dynamic_cast<FiniteHorizonScheduler*>(scheduler);
        fhScheduler->scheduleUL(ULdemandVec, ULallocVec, userCnt);

        i = 0;
        // Execute UL scheduling & transmission
        for(it = users.begin(); it != users.end(); it++)
        {
            int userID = (*it)->getID();

            if (ULallocVec[userID] > 0 && ULdemandVec[userID] > 0)
            {
                (*it)->scheduledCntr++;
                if(uniDistribution(generator) > (*it)->lossProbability)
                {
                    txedPkt = (*it)->transmit();
                    txSuccess = true;
                }
            }
            // Count loop
            if((*it)->transmitter)
                i++;
        }

        // Reception at the end of the slot
        if(txSuccess)
        {
            getUserWithID(txedPkt.destinationID)->receive(txedPkt);

            fhScheduler->update(txedPkt);
        }

        break;
    }

    default:
        break;
    }
}

