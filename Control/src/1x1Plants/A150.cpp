#include "Control/1x1Plants/A150.h"
#include "Common/helpers.h"


A150::A150()
    : ControlLoop<A150Types::typeA, A150Types::typeB,
      A150Types::typeX, A150Types::typeC,
      A150Types::typeY, A150Types::typeU,
      A150Types::typeK, A150Types::typeL,
      A150Types::typeR>(A150_A,    A150_B, A150_X0,   A150_C,
                        A150_K,    A150_L, A150_REF,  A150_SAMPLING_PERIOD_MS,
                        A150_GAUSSIAN_NOISE_MEAN, A150_GAUSSIAN_NOISE_STDDEV)
{

}


const A150Types::typeA A150_A = (A150Types::typeA() << 1.50).finished();

const A150Types::typeB A150_B = (A150Types::typeB() <<  1.0).finished();

const A150Types::typeC A150_C = A150Types::typeC::Identity();

const A150Types::typeX A150_X0 = (A150Types::typeX() << 0.0).finished();

const A150Types::typeK A150_K = (A150Types::typeK() << 0.0).finished();

const A150Types::typeL A150_L = (A150Types::typeL() << 1.50).finished();

const A150Types::typeR A150_REF = (A150Types::typeR() << 0.0).finished();

const unsigned short A150_SAMPLING_PERIOD_MS = 3;


const A150Types::typeX A150_GAUSSIAN_NOISE_MEAN = (A150Types::typeX() << 0.0).finished();

const A150Types::typeX A150_GAUSSIAN_NOISE_STDDEV = (A150Types::typeX() << 1.0).finished();
