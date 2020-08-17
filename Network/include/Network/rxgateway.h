/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef RXGATEWAY_H
#define RXGATEWAY_H

#include "Common/link.h"
#include "basestation.h"
#include "NetworkBase/gateway.h"
#include "NetworkBase/packet.h"

template <typename typeOut>
class RxGateway : public Gateway
{
public:
    RxGateway();
    void setOutLink(Link<typeOut> * const l);
    void receiveFromBS(const int &k);

    double totalDelay;
    void startMeasuring();

private:
    Link<typeOut>* gwToPlantLink;
};


template <typename typeOut>
RxGateway<typeOut>::RxGateway()
{
    totalDelay = 0.0;
}

template<typename typeOut>
void RxGateway<typeOut>::setOutLink(Link<typeOut> * const l)
{
    gwToPlantLink = l;
}

/**
 * Empty the receive queue and forward the most recent packet in the queue to the GwToPlant link
 */
template<typename typeOut>
void RxGateway<typeOut>::receiveFromBS(const int & k)
{
    int most_recent_pkt = -1;
    typeOut outVec;


    if(rxBuf.dataLen != 0 && rxBuf.destinationID == networkID)
    {
        assert(rxBuf.dataLen == gwToPlantLink->readValue().rows());

        most_recent_pkt = rxBuf.timeStamp;
        for(int r = 0; r < outVec.rows(); r++)
            outVec(r, 0) = rxBuf.data[r];

        if(recordStats_MT) {
            totalDelay += k - rxBuf.timeStamp;
            rcvdMsgCntr ++;
        }

        rxBuf.dataLen = 0;      // Mark packet as read
    }

    if(most_recent_pkt >= 0)
    {
        gwToPlantLink->setValue(outVec);
        gwToPlantLink->setTimeStamp(most_recent_pkt);
    }
}

template<typename typeOut>
void RxGateway<typeOut>::startMeasuring()
{
    recordStats_MT = true;
}


#endif // RXGATEWAY_H
