/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <map>

enum SchedulerTypes {AOI, FINITE_HORIZON_SCHEDULER};

extern const double f_C_Hz;

extern const double f_C_GHz;

// The number of "Usable" Resource Blocks allowed in each bandwidth for uplink and downlink
extern const std::map<const double, const int> numPRB;

extern const double channelBandwidth_MHz;

extern const int RBsBorrowedFromDownlinkByUplink;

extern const int ULnumPRB;

extern const int DLnumPRB;

extern const unsigned int resourceDuration_ms;

extern const double resourceBandwidth_kHz;

extern const double resourceBandwidth_Hz;

extern const double h_BS;       // Base Station antenna height

extern const double h_BS_eff_m;

extern const double minimumUE2BSDistance_m;

extern const double maximumUE2BSDistance_m;

extern const double UEmobility_kmh;

extern const double BSTxPower_dBm;

extern const double UETxPower_dBm;

extern const int coherenceTime_ms;

extern const unsigned maxAoI;

extern const double c_mps;

#define dataratePerRB 120.0

#define DLdataratePerRB dataratePerRB

extern const int d_UL;

extern const int d_DL;


#endif // NETWORKCONFIG_H
