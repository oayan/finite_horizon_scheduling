#include "aoischeduler.h"
#include <iostream>
#include "NetworkBase/packet.h"
#include <math.h>
#include <assert.h>

void AOIScheduler::scheduleUL(const int * demandVec, int * allocVec, const int &userCnt)
{
    int totalRemainingDemand = 0;

    int numRemainingResources = ULnumRB;

    totalULlresources += ULnumRB;

    int remainingDemandVec[userCnt];

    for(int i = 0; i < userCnt; i++)
    {
        totalRemainingDemand += demandVec[i];
        remainingDemandVec[i] = demandVec[i];

    }
    double highestULUtility = 0;
    int highestULutilityIndex = 0;
    double utility = 0.0;
    AoIApplication* highestULutilityApp;

    // Keep allocating UL resources as long as all of the following hold:
    // 1) there are remaining UL resources left to allocate
    // 2) there is UL demand left to schedule
    while(numRemainingResources > 0 && totalRemainingDemand > 0)
    {
        highestULUtility = 0.0;
        highestULutilityIndex = 0;

        // Look for UL packet with the highest utility
        for(int i = 1; i <= userCnt; i++)
        {
            if(remainingDemandVec[i] > 0)
            {
                AoIApplication* app = getAppByTxId(i);

                utility = app->getAgeInXSlots(0);

                if(utility > highestULUtility)
                {
                    highestULutilityIndex = i;
                    highestULUtility = utility;
                    highestULutilityApp = app;
                }
            }
        }

        // If none found, then totalRemainingDemand should have been empty !! Double check
        if(highestULUtility == 0.0)
        {
            break;
        }

        int allocatedResources = std::min(numRemainingResources, remainingDemandVec[highestULutilityIndex]);
        remainingDemandVec[highestULutilityIndex] -= allocatedResources;
        allocVec[highestULutilityIndex] = allocatedResources;
        usedULresources += allocatedResources;
        numRemainingResources -= allocatedResources;
        totalRemainingDemand -= allocatedResources;

    } // End of while loop

    wastedULresources += numRemainingResources;
    assert(numRemainingResources >= 0);
}


void AOIScheduler::update(const packet & pkt)
{
    std::vector<AoIApplication>::iterator it;
    // Collect resource requests
    for(it = aoiApps.begin(); it != aoiApps.end(); it++)
    {
        if((*it).getReceiverID() == pkt.destinationID)
        {
            (*it).l = pkt.timeStamp;
            (*it).setAge((*it).k - (*it).getLatestSeenTimestamp());
            return;
        }
    }
}

void AOIScheduler::timePass_1ms(int t)
{
    std::vector<AoIApplication>::iterator it;
    // Collect resource requests
    for(it = aoiApps.begin(); it != aoiApps.end(); it++)
    {
        (*it).update();
    }
}


AoIApplication * AOIScheduler::getAppByTxId(int txID)
{
    std::vector<AoIApplication>::iterator it;
    // Collect resource requests
    for(it = aoiApps.begin(); it != aoiApps.end(); it++)
    {
        if((*it).getTransmitterID() == txID)
            return &(*it);
    }

    return NULL;
}

AoIApplication* AOIScheduler::getAppByRxId(int rxId)
{
    std::vector<AoIApplication>::iterator it;
    // Collect resource requests
    for(it = aoiApps.begin(); it != aoiApps.end(); it++)
    {
        if((*it).getReceiverID() == rxId)
            return &(*it);
    }

}

void AOIScheduler::registerApp(MobileTerminal * const tx, MobileTerminal * const rx, const int & ts_ms, const int & time_until_next_sample_ms)
{
    aoiApps.push_back(AoIApplication(tx, rx, ts_ms, time_until_next_sample_ms));
}
