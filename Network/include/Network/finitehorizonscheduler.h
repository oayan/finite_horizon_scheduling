/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef FINITEHORIZONSCHEDULER_H
#define FINITEHORIZONSCHEDULER_H

#include "NetworkBase/scheduler.h"
#include "controlapplication.h"

extern const int maximumHorizon;

class FiniteHorizonScheduler : public Scheduler
{
public:
    FiniteHorizonScheduler();

    void scheduleUL(const int *demandVec, int *allocVec, const int &userCnt) override;

    ~FiniteHorizonScheduler();

    template<typename typeA, typename typeB, typename typeL>
    void registerLoop(MobileTerminal * const txGw,
                      MobileTerminal * const rxGw,
                      const typeA & _A,
                      const typeB & _B,
                      const typeL & _L,
                      const typeA & _W,
                      const int & ts_ms,
                      int t_until_next_sample_ms);

    void updateULUtility(ControlApplication * const app);

    void timePass_1ms(int t);

    void initStates() override;

    void initLossProbabilties();

    void initCostMap();

    void update(const packet & pkt);

    ControlApplication *getAppByTxId(int txID);

    float getAvgNodeCount() { return (float)totalNodeCount / numNodeCountMeasurements;}

private:
    std::vector<ControlApplication*> controlLoops;

    double** costMap;

    int* samplingOffsets;

    int* samplingPeriods;

    int numNodeCountMeasurements;

    unsigned long totalNodeCount;

};

template<typename typeA, typename typeB, typename typeL>
void FiniteHorizonScheduler::registerLoop(MobileTerminal * const txGw,
                                          MobileTerminal * const rxGw,
                                          const typeA & _A,
                                          const typeB & _B,
                                          const typeL & _L,
                                          const typeA & _W,
                                          const int & ts_ms,
                                          int samplingOffset)
{
    ControlApplication* newLoop = new ControlApplication(txGw, rxGw);
    newLoop->initControl(_A, _B, _L, _W, ts_ms, samplingOffset);
    controlLoops.push_back(newLoop);
}

#endif // FINITEHORIZONSCHEDULER_H
