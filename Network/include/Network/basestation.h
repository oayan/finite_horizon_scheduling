/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef BASESTATION_H
#define BASESTATION_H

#include <vector>
#include "NetworkBase/mobileterminal.h"
#include "networkconfig.h"
#include <iostream>
#include "aoischeduler.h"
#include "finitehorizonscheduler.h"


#define BS_MAX_USERS 251

class BaseStation
{
public:
    BaseStation(SchedulerTypes st);

    ~BaseStation();

    template<typename typeA, typename typeB, typename typeL>
    void registerLoop(MobileTerminal * const txGw, MobileTerminal * const rxGw, const typeA & _A, const typeB & _B, const typeL & _L, const typeA & _W, const int & ts_ms, int samplingOffset);

    void update(int t);

    int addUser(MobileTerminal * const mt);

    void init_scheduler();

    //private:
    const SchedulerTypes schType;

    Scheduler* scheduler;

    std::vector<MobileTerminal*> users;

    std::vector<int> availableNetworkIDs;

    MobileTerminal * getUserWithID(const int & userID);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> uniDistribution;


};

template<typename typeA, typename typeB, typename typeL>
void BaseStation::registerLoop(MobileTerminal * const txGw, MobileTerminal * const rxGw, const typeA & _A, const typeB & _B, const typeL & _L, const typeA & _W, const int & ts_ms, int samplingOffset)
{
    switch (schType)
    {

    case AOI:
    {
        AOIScheduler* AoIscheduler = dynamic_cast<AOIScheduler *>(scheduler);
        addUser(txGw);
        addUser(rxGw);
        AoIscheduler->registerApp(txGw, rxGw, ts_ms, samplingOffset);
        break;
    }
    case FINITE_HORIZON_SCHEDULER:
    {
        FiniteHorizonScheduler* fhScheduler = dynamic_cast<FiniteHorizonScheduler*>(scheduler);
        addUser(txGw);
        addUser(rxGw);
        fhScheduler->registerLoop(txGw, rxGw, _A, _B, _L, _W, ts_ms, samplingOffset);
        break;
    }
    default:
    {
        addUser(txGw);
        addUser(rxGw);

        //std::cerr << "BS is registering control application to control unaware scheduler" << std::endl;
        break;
    }
    }
}


#endif // BASESTATION_H
