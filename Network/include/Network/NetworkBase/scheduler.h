/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Network/networkconfig.h"


class Scheduler
{
public:
    Scheduler()
    {
        ULnumRB = ULnumPRB;
        wastedULresources = 0;
        totalULlresources = 0;
        usedULresources = 0;
        states = NULL;
        t = 0;
    }

    virtual ~Scheduler()
    {
        if(lossProbabilities != NULL)
        {
            delete [] lossProbabilities;
            lossProbabilities = NULL;
        }
    }
    virtual void scheduleUL(const int * demandVec, int * allocVec, const int & userCnt) = 0;

    virtual void initStates() = 0;

    //protected:
    int t;
    int* states;

    double* lossProbabilities;

    unsigned N;

    int ULnumRB;
    int wastedULresources;
    int totalULlresources;
    int usedULresources;
};

#endif // SCHEDULER_H
