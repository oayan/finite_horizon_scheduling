/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef A075_H
#define A075_H

#include "Control/ControlBase/controlloop.h"

#define A075_n 1
#define A075_m 1
#define A075_r 1

namespace A075Types {
using typeA = Eigen::Matrix<double, A075_n, A075_n>;
using typeB = Eigen::Matrix<double, A075_n, A075_m>;
using typeX = Eigen::Matrix<double, A075_n, 1>;
using typeC = Eigen::Matrix<double, A075_r, A075_n>;
using typeY = Eigen::Matrix<double, A075_r, 1>;
using typeU = Eigen::Matrix<double, A075_m, 1>;
using typeK = Eigen::Matrix<double, 1, 1>;
using typeL = Eigen::Matrix<double, 1, A075_n>;
using typeR = Eigen::Matrix<double, 1, 1>;
}


extern const A075Types::typeA A075_A;
extern const A075Types::typeB A075_B;
extern const A075Types::typeC A075_C;
extern const A075Types::typeX A075_X0;
extern const A075Types::typeK A075_K;
extern const A075Types::typeL A075_L;
extern const A075Types::typeR A075_REF;
extern const unsigned short A075_SAMPLING_PERIOD_MS;

extern const A075Types::typeX A075_GAUSSIAN_NOISE_MEAN;
extern const A075Types::typeX A075_GAUSSIAN_NOISE_STDDEV;


class A075 : public ControlLoop<A075Types::typeA, A075Types::typeB,
                                            A075Types::typeX, A075Types::typeC,
                                            A075Types::typeY, A075Types::typeU,
                                            A075Types::typeK, A075Types::typeL,
                                            A075Types::typeR>
{
public:
    A075();
};

#endif // A075_H
