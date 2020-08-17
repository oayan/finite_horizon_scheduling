#include "mobileterminal.h"
#include <cstddef>
#include <iostream>
#include "networkconfig.h"
#include <math.h>       /* sqrt, exp, pow */
#include <chrono>
#include <assert.h>     /* assert */
#include "Common/noise.h"
#include <math.h>
#include "Common/helpers.h"
#include <algorithm>    // std::max

MobileTerminal::MobileTerminal()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    shadowing_dB = std::normal_distribution<double>(0.0, 1.0);
    O2CAR_dB = std::normal_distribution<double>(9.0, 1.0);
    customlossDistribution = std::normal_distribution<double>(0.3, 0.2);

    // UE Antenna height
    h_e_m = 1.0;
    h_UT_m = 1.5;
    h_UT_eff_m = h_UT_m - h_e_m;

    d_BP_m = 4.0 * h_BS_eff_m * h_UT_eff_m * f_C_Hz / c_mps;

    d2D = 1500;

    // Pathloss
    PL_dB = 32.4 + 40.0 * log10(d2D) + 20.0 * log10(f_C_GHz) - 9.5 * log10(pow(d_BP_m, 2) + pow(h_BS - h_UT_m, 2));

    // Outdoor to indoor (car)
    PL_dB += O2CAR_dB(generator);

    ulNoisePower_dBm = -174.0 + 10.0 * log10(resourceBandwidth_Hz);

    updateLossProbability();
    constantChannelDuration_ms = rand() % coherenceTime_ms + 1;

    networkID = 0;
    ULDataPending = false;
    DLDataPending = false;

    ulSNR_dB = 0.0;
    dlSNR_dB = 0.0;

    rcvdMsgCntr = 0;
    scheduledCntr = 0;
    txedMsgCntr = 0;
    recordStats_MT = false;

    rxBuf.timeStamp = -1;
    rxBuf.data[0] = {0};
    rxBuf.dataLen = 0;
}

void MobileTerminal::push(const packet & pkt)
{
    txBuf = pkt;
    requestUplink();
}

void MobileTerminal::setID(int id)
{
    networkID = id;
}

int MobileTerminal::getID()
{
    return networkID;
}

bool MobileTerminal::isRequestingUplink()
{
    return ULDataPending;
}

void MobileTerminal::setUplinkDataPending(bool isDataPending)
{
    ULDataPending = isDataPending;
}

void MobileTerminal::setDownlinkDataPending(bool isDataPending)
{
    DLDataPending = isDataPending;
}


double MobileTerminal::getd2D() const
{
    return d2D;
}

double MobileTerminal::getd3D() const
{
    return d3D;
}




packet MobileTerminal::transmit()
{
    packet p = txBuf;
//    sqDebug() << "MobileTerminal: transmit(): " << " Packet transmitted with k: " << p.timeStamp;

    if(recordStats_MT)
        txedMsgCntr++;

    setUplinkDataPending(false);

    return p;
}

void MobileTerminal::receive(const packet pkt)
{
    rxBuf = pkt;
}


bool MobileTerminal::timePass_1ms()
{
    if(constantChannelDuration_ms-- == 0) {
        constantChannelDuration_ms = coherenceTime_ms - 1;
        updateLossProbability();
        return true;
    }
    return false;
}

bool MobileTerminal::isDownlinkDataPending()
{
    return DLDataPending;
}

void MobileTerminal::requestUplink()
{
    ULDataPending = true;
}

void MobileTerminal::updateLossProbability()
{
//    double attenuation_dB = PL_dB + shadowing_dB(generator);

//    double P_s_dBm = UETxPower_dBm - attenuation_dB;

//    double SNR_dB = P_s_dBm - ulNoisePower_dBm;

//    // SNR to BER
//    double bpsk_ber = Q(sqrt(2.0 * dB_to_linear(SNR_dB)));

//    // BER to PER
//    double packet_size_bits = PACKET_SIZE_IN_REAL_BYTES * 8.0;

//    lossProbability = 1.0 - pow(1.0 - bpsk_ber, packet_size_bits);
    lossProbability = std::min(std::max(customlossDistribution(generator), 0.0), 1.0);
}



