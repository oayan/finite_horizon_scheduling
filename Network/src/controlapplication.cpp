#include "controlapplication.h"

ControlApplication::ControlApplication(MobileTerminal * const tx, MobileTerminal * const rx)
    : Application(tx, rx)
{
    k = 0;
    m = -1;
    ULutility = 1.0;
    AoI = k - m;
}


ControlApplication::~ControlApplication()
{
}

MatrixXd ControlApplication::getApower(int pow)
{
    MatrixPower<MatrixXd>Apow(A);
    return Apow(pow);
}

int ControlApplication::getAoi() const
{
    return AoI;
}


int ControlApplication::getAgeInXSlots(int x)
{
    if (x == 0)
        return AoI;

    int periods = static_cast<int>(floor(double(x) / double(sampling_period_ms)));
    int remain = x % sampling_period_ms;

    if(remain <= t_until_next_sample_ms)
        return AoI + periods + 1;
    else
        return AoI + periods;
}

bool ControlApplication::updateBuffer(const packet & newPkt)
{
    if(newPkt.dataLen == x_m.rows() && newPkt.timeStamp > m)
    {
        m = newPkt.timeStamp;
        for(int i = 0; i < newPkt.dataLen; i++)
            x_m(i) = newPkt.data[i];
        return true;
    }
    return false;
}


double ControlApplication::getULUtility() const
{
    return ULutility;
}

void ControlApplication::setULUtility(double util)
{
    ULutility = util;
}


bool ControlApplication::timePass_1ms(int t)
{
    if(t_until_next_sample_ms-- == 0)
    {
        t_until_next_sample_ms = sampling_period_ms - 1;
        return true;
    }
    return false;
}

void ControlApplication::mimicSampling()
{
    k++;
    AoI = k - m;
    assert(AoI >= 0);
}

int ControlApplication::getSamplingOffset() const
{
    return samplingOffset_ms;
}
