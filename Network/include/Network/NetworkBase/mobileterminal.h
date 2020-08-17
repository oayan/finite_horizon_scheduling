/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef MOBILETERMINAL_H
#define MOBILETERMINAL_H

#include "packet.h"
#include <queue>
#include <random>

class MobileTerminal
{
public:
    MobileTerminal();

    static void copyPacket(packet & dest, const packet & src) {std::memcpy(&dest, &src, sizeof(packet));}

    void push(const packet & pkt);

    void setID(int id);

    int getID();

    bool isRequestingUplink();

    bool isDownlinkDataPending();

    void setUplinkDataPending(bool isDataPending);

    void setDownlinkDataPending(bool isDataPending);

    double getd2D() const;

    double getd3D() const;

    bool timePass_1ms();

    packet transmit();

    void receive(const packet pkt);

    int rcvdMsgCntr;

    int txedMsgCntr;

    int scheduledCntr;

    packet txBuf;

    packet rxBuf;

    void requestUplink();

    int networkID;
    int partnerID;
    bool transmitter;
    bool recordStats_MT;

    double lossProbability;

private:
    bool ULDataPending;
    bool DLDataPending;
    double d2D;             // 2D Distance to BS
    double d2D_in;          // Indoor Distance from UE to building
    double d2D_out;         // Outdoor Distance from building to BS
    double d3D;             // 3D Distance to BS
    bool indoor;            // True = Indoor <> False = Outdoor
    bool los;               // True = Line Of Sight <> False = NLOS
    double h_UT_m;            // Antenna height in meters
    double h_e_m;
    double h_UT_eff_m;      // Effective antenna height
    double d_BP_m;          // Breakpoint distance in meters
    double PL_dB;           // Path Loss in dB (constant over time)
    double sigmaSF;
    int constantChannelDuration_ms;
    double ulSNR_dB;
    double dlSNR_dB;
    double ulNoisePower_dBm;
    double dlNoisePower_dBm;


    std::default_random_engine generator;
    std::normal_distribution<double> shadowing_dB;
    std::normal_distribution<double> customlossDistribution;
    std::normal_distribution<double> O2CAR_dB;
    void updateLossProbability();

};

#endif // MOBILETERMINAL_H
