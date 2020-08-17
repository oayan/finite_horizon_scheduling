/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef GATEWAY_H
#define GATEWAY_H

#include "mobileterminal.h"

class Gateway : public MobileTerminal
{
public:
    Gateway();

    int getPartnerID();
    void setPartnerID(int ID);

};

#endif // GATEWAY_H
