#include "gateway.h"

Gateway::Gateway()
{
    partnerID = 0;
}

int Gateway::getPartnerID()
{
    return partnerID;
}

void Gateway::setPartnerID(int ID)
{
    partnerID = ID;
}
