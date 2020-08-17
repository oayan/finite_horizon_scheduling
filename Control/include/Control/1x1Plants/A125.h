/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef A125_H
#define A125_H

#include "Control/ControlBase/controlloop.h"

#define A125_n 1
#define A125_m 1
#define A125_r 1

namespace A125Types {
using typeA = Eigen::Matrix<double, A125_n, A125_n>;
using typeB = Eigen::Matrix<double, A125_n, A125_m>;
using typeX = Eigen::Matrix<double, A125_n, 1>;
using typeC = Eigen::Matrix<double, A125_r, A125_n>;
using typeY = Eigen::Matrix<double, A125_r, 1>;
using typeU = Eigen::Matrix<double, A125_m, 1>;
using typeK = Eigen::Matrix<double, 1, 1>;
using typeL = Eigen::Matrix<double, 1, A125_n>;
using typeR = Eigen::Matrix<double, 1, 1>;
}


extern const A125Types::typeA A125_A;
extern const A125Types::typeB A125_B;
extern const A125Types::typeC A125_C;
extern const A125Types::typeX A125_X0;
extern const A125Types::typeK A125_K;
extern const A125Types::typeL A125_L;
extern const A125Types::typeR A125_REF;
extern const unsigned short A125_SAMPLING_PERIOD_MS;

extern const A125Types::typeX A125_GAUSSIAN_NOISE_MEAN;
extern const A125Types::typeX A125_GAUSSIAN_NOISE_STDDEV;


class A125 : public ControlLoop<A125Types::typeA, A125Types::typeB,
                                            A125Types::typeX, A125Types::typeC,
                                            A125Types::typeY, A125Types::typeU,
                                            A125Types::typeK, A125Types::typeL,
                                            A125Types::typeR>
{
public:
    A125();
};

#endif // A125_H
