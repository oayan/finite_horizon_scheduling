/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef AOIAPPLICATION_H
#define AOIAPPLICATION_H

#include "NetworkBase/application.h"

class AoIApplication : public Application
{
public:
    AoIApplication(MobileTerminal * const tx, MobileTerminal * const rx, const int ts_ms, const int sampling_offset);

    void update();

    int getAge() const;
    void setAge(int newAge);

    int getLatestSeenTimestamp();

    int getAgeInXSlots(int x) const;

    int l;
    int k;

private:
    int age;
    int t_until_next_sample_ms;
    int ts_ms;
};

#endif // AOIAPPLICATION_H
