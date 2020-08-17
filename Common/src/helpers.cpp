#include "helpers.h"
#include <math.h>


double dB_to_linear(double dB)
{
    return pow(10.0, dB / 10.0);
}

double linear_to_dB(double linear)
{
    return 10.0 * log10(linear);
}

double mW_to_dBm(double mW)
{
    return linear_to_dB(mW);
}

double dBm_to_mW(double dBm)
{
    return dB_to_linear(dBm);
}

double Q(double x)
{
    return 0.5 * erfc(x / sqrt(2));
}

