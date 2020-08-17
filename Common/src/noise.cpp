#include "noise.h"
#include <math.h>


double Noise::getNoisePower_dBm(double noiseFloor_dBm_per_Hz, double bandwidth_Hz, double noiseFigure_dB)
{
    return noiseFloor_dBm_per_Hz + (10.0 * log10(bandwidth_Hz)) + noiseFigure_dB;
}
