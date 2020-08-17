/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef NOISE_H
#define NOISE_H

// Noise floor at 290K temperature
const double noiseFloor_290K_dBm_per_HZ = -174.0;

// Noise figure when BS is receiving (Uplink)
const double BS_noiseFigure_dB = 2.0;

//Noise fogure when UE is receiving (Downlink)
const double UE_noiseFigure_dB = 7.0;

class Noise {

public:
    static double getNoisePower_dBm(double noiseFloor_dBm_per_Hz, double bandwidth_Hz, double noiseFigure_dB);
protected:
    virtual void generateNoise() = 0;
};


#endif // NOISE_H
