/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef APPLICATION_H
#define APPLICATION_H

#include "mobileterminal.h"
#include "packet.h"

class Application
{
public:
    Application(MobileTerminal * tx, MobileTerminal * rx) : transmitter(tx), receiver(rx) { }

    int getTransmitterID() const {return transmitter->getID();}

    int getReceiverID() const {return receiver->getID();}

    void updateBuffer(const packet & newPkt) {MobileTerminal::copyPacket(pktBuf, newPkt);}

    double getLossProbability() const {return transmitter->lossProbability;}

    const MobileTerminal* getTransmitter() const {return transmitter;}

protected:
    MobileTerminal * const transmitter;

    MobileTerminal * const receiver;

    packet pktBuf;      // packet buf of 1 packet per flow

};

#endif // APPLICATION_H
