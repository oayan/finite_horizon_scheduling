#include "Control/1x1Plants/A125.h"
#include "Common/helpers.h"


A125::A125()
    : ControlLoop<A125Types::typeA, A125Types::typeB,
      A125Types::typeX, A125Types::typeC,
      A125Types::typeY, A125Types::typeU,
      A125Types::typeK, A125Types::typeL,
      A125Types::typeR>(A125_A,    A125_B, A125_X0,   A125_C,
                        A125_K,    A125_L, A125_REF,  A125_SAMPLING_PERIOD_MS,
                        A125_GAUSSIAN_NOISE_MEAN, A125_GAUSSIAN_NOISE_STDDEV)
{ }


const A125Types::typeA A125_A = (A125Types::typeA() << 1.25).finished();

const A125Types::typeB A125_B = (A125Types::typeB() <<  1.0).finished();

const A125Types::typeC A125_C = A125Types::typeC::Identity();

const A125Types::typeX A125_X0 = (A125Types::typeX() << 0.0).finished();

const A125Types::typeK A125_K = (A125Types::typeK() << 0.0).finished();

const A125Types::typeL A125_L = (A125Types::typeL() << 1.25).finished();

const A125Types::typeR A125_REF = (A125Types::typeR() << 0.0).finished();

const unsigned short A125_SAMPLING_PERIOD_MS = 3;


const A125Types::typeX A125_GAUSSIAN_NOISE_MEAN = (A125Types::typeX() << 0.0).finished();

const A125Types::typeX A125_GAUSSIAN_NOISE_STDDEV = (A125Types::typeX() << 1.0).finished();
