#include "Control/1x1Plants/A100.h"
#include "Common/helpers.h"


A100::A100()
    : ControlLoop<A100Types::typeA, A100Types::typeB,
      A100Types::typeX, A100Types::typeC,
      A100Types::typeY, A100Types::typeU,
      A100Types::typeK, A100Types::typeL,
      A100Types::typeR>(A100_A,    A100_B, A100_X0,   A100_C,
                        A100_K,    A100_L, A100_REF,  A100_SAMPLING_PERIOD_MS,
                        A100_GAUSSIAN_NOISE_MEAN, A100_GAUSSIAN_NOISE_STDDEV)
{

}


const A100Types::typeA A100_A = (A100Types::typeA() << 1.00).finished();

const A100Types::typeB A100_B = (A100Types::typeB() <<  1.0).finished();

const A100Types::typeC A100_C = A100Types::typeC::Identity();

const A100Types::typeX A100_X0 = (A100Types::typeX() << 0.0).finished();

const A100Types::typeK A100_K = (A100Types::typeK() << 0.0).finished();

const A100Types::typeL A100_L = (A100Types::typeL() << 1.00).finished();

const A100Types::typeR A100_REF = (A100Types::typeR() << 0.0).finished();

const unsigned short A100_SAMPLING_PERIOD_MS = 3;


const A100Types::typeX A100_GAUSSIAN_NOISE_MEAN = (A100Types::typeX() << 0.0).finished();

const A100Types::typeX A100_GAUSSIAN_NOISE_STDDEV = (A100Types::typeX() << 1.0).finished();
