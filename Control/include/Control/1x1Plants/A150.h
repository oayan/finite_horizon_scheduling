/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef A150_H
#define A150_H

#include "Control/ControlBase/controlloop.h"

#define A150_n 1
#define A150_m 1
#define A150_r 1

namespace A150Types {
using typeA = Eigen::Matrix<double, A150_n, A150_n>;
using typeB = Eigen::Matrix<double, A150_n, A150_m>;
using typeX = Eigen::Matrix<double, A150_n, 1>;
using typeC = Eigen::Matrix<double, A150_r, A150_n>;
using typeY = Eigen::Matrix<double, A150_r, 1>;
using typeU = Eigen::Matrix<double, A150_m, 1>;
using typeK = Eigen::Matrix<double, 1, 1>;
using typeL = Eigen::Matrix<double, 1, A150_n>;
using typeR = Eigen::Matrix<double, 1, 1>;
}


extern const A150Types::typeA A150_A;
extern const A150Types::typeB A150_B;
extern const A150Types::typeC A150_C;
extern const A150Types::typeX A150_X0;
extern const A150Types::typeK A150_K;
extern const A150Types::typeL A150_L;
extern const A150Types::typeR A150_REF;
extern const unsigned short A150_SAMPLING_PERIOD_MS;

extern const A150Types::typeX A150_GAUSSIAN_NOISE_MEAN;
extern const A150Types::typeX A150_GAUSSIAN_NOISE_STDDEV;


class A150 : public ControlLoop<A150Types::typeA, A150Types::typeB,
                                            A150Types::typeX, A150Types::typeC,
                                            A150Types::typeY, A150Types::typeU,
                                            A150Types::typeK, A150Types::typeL,
                                            A150Types::typeR>
{
public:
    A150();
};

#endif // A150_H
