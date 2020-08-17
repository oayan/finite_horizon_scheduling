#include "Control/1x1Plants/A075.h"
#include "Common/helpers.h"


A075::A075()
    : ControlLoop<A075Types::typeA, A075Types::typeB,
      A075Types::typeX, A075Types::typeC,
      A075Types::typeY, A075Types::typeU,
      A075Types::typeK, A075Types::typeL,
      A075Types::typeR>(A075_A,    A075_B, A075_X0,   A075_C,
                        A075_K,    A075_L, A075_REF,  A075_SAMPLING_PERIOD_MS,
                        A075_GAUSSIAN_NOISE_MEAN, A075_GAUSSIAN_NOISE_STDDEV)
{

}


const A075Types::typeA A075_A = (A075Types::typeA() << 0.75).finished();

const A075Types::typeB A075_B = (A075Types::typeB() <<  1.0).finished();

const A075Types::typeC A075_C = A075Types::typeC::Identity();

const A075Types::typeX A075_X0 = (A075Types::typeX() << 0.0).finished();

const A075Types::typeK A075_K = (A075Types::typeK() << 0.0).finished();

const A075Types::typeL A075_L = (A075Types::typeL() << 0.75).finished();

const A075Types::typeR A075_REF = (A075Types::typeR() << 0.0).finished();

const unsigned short A075_SAMPLING_PERIOD_MS = 3;


const A075Types::typeX A075_GAUSSIAN_NOISE_MEAN = (A075Types::typeX() << 0.0).finished();

const A075Types::typeX A075_GAUSSIAN_NOISE_STDDEV = (A075Types::typeX() << 1.0).finished();
