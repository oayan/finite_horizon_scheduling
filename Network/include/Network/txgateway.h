/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef TXGATEWAY_H
#define TXGATEWAY_H

#include "Common/link.h"
#include "NetworkBase/gateway.h"
#include <iostream>
#include "networkconfig.h"

template <typename typeIn>
class TxGateway : public Gateway
{
public:
    TxGateway() {};
    void setInputLink(Link<typeIn>* const l);
    void recvFromPlant();
    void startMeasuring();

private:
    Link<typeIn>* sensorToGwLink;
};


template<typename typeIn>
void TxGateway<typeIn>::setInputLink(Link<typeIn> * const l)
{
    sensorToGwLink = l;
}

template<typename typeIn>
void TxGateway<typeIn>::recvFromPlant()
{
    typeIn val = sensorToGwLink->readValue();
    int timestamp = sensorToGwLink->readTimeStamp();
    packet p;

    // Pack data into packet
    p.timeStamp = timestamp;
    p.dataLen = val.rows();
    p.destinationID = partnerID;

    for (int r = 0; r < p.dataLen; r++)
        p.data[r] = val(r, 0);

    txBuf = p;

    setUplinkDataPending(true);

//    qDebug() << "TXGW: " << networkID << " Packet waiting with k: " << p.timeStamp;


    //std::cout << "TXGW: " << networkID << " Packet waiting with k: " << p.timeStamp << std::endl;
}

template<typename typeIn>
void TxGateway<typeIn>::startMeasuring()
{
    recordStats_MT = true;
}




#endif // TXGATEWAY_H
