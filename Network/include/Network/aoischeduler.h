/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef AOISCHEDULER_H
#define AOISCHEDULER_H

#include "NetworkBase/scheduler.h"
#include "aoiapplication.h"

class AOIScheduler : public Scheduler
{
public:
    AOIScheduler() : Scheduler() {};
    ~AOIScheduler() {aoiApps.clear();}

    void scheduleUL(const int * demandVec, int * allocVec, const int & userCnt) override;

    void registerApp(MobileTerminal * const tx, MobileTerminal * const rx, const int & ts_ms, const int & time_until_next_sample_ms);

    void update(const packet & pkt);

    void timePass_1ms(int t);

    void initStates() override {N = aoiApps.size();}

    void initLossProbabilties()
    {
        for (unsigned i = 0; i < N; ++i)
            lossProbabilities[i] = aoiApps.at(i).getTransmitter()->lossProbability;
    }

    std::vector<AoIApplication> aoiApps;

    AoIApplication * getAppByTxId(int txID);
    AoIApplication * getAppByRxId(int rxId);
};

#endif // AOISCHEDULER_H
