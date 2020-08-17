#include "networkconfig.h"

const double f_C_GHz = 2.0;

const double f_C_Hz = f_C_GHz * 1000000000.0;

const double c_mps = 300000000.0;

// The number of "Usable" Resource Blocks allowed in each bandwidth for uplink and downlink
const std::map<const double, const int> numPRB{{0.18, 1}, {0.60, 3}, {1.4, 6}, {3.0, 15}, {5.0, 25}, {10.0, 50}, {15.0, 75}, {20.0, 100}};

const double channelBandwidth_MHz = 0.60;

const unsigned int resourceDuration_ms = 1;

const int RBsBorrowedFromDownlinkByUplink = 0;

const int ULnumPRB = 1;

const int DLnumPRB = 1;

const double resourceBandwidth_kHz = 180.0;

const double resourceBandwidth_Hz = resourceBandwidth_kHz * 1000.0;

const double h_BS = 10.0;

const double h_BS_eff_m = 9.0;

const double minimumUE2BSDistance_m = 10.0;

const double maximumUE2BSDistance_m = 5000.0;

const double UEmobility_kmh = 3.0;

const double BSTxPower_dBm = 41.0;

const double UETxPower_dBm = 20.0;

const int coherenceTime_ms = 30;

const int d_UL = 1;

const int d_DL = 1;

const unsigned maxAoI = 100;

