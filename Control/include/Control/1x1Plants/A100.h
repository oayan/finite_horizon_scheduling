/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef A100_H
#define A100_H

#include "Control/ControlBase/controlloop.h"

#define A100_n 1
#define A100_m 1
#define A100_r 1

namespace A100Types {
using typeA = Eigen::Matrix<double, A100_n, A100_n>;
using typeB = Eigen::Matrix<double, A100_n, A100_m>;
using typeX = Eigen::Matrix<double, A100_n, 1>;
using typeC = Eigen::Matrix<double, A100_r, A100_n>;
using typeY = Eigen::Matrix<double, A100_r, 1>;
using typeU = Eigen::Matrix<double, A100_m, 1>;
using typeK = Eigen::Matrix<double, 1, 1>;
using typeL = Eigen::Matrix<double, 1, A100_n>;
using typeR = Eigen::Matrix<double, 1, 1>;
}


extern const A100Types::typeA A100_A;
extern const A100Types::typeB A100_B;
extern const A100Types::typeC A100_C;
extern const A100Types::typeX A100_X0;
extern const A100Types::typeK A100_K;
extern const A100Types::typeL A100_L;
extern const A100Types::typeR A100_REF;
extern const unsigned short A100_SAMPLING_PERIOD_MS;

extern const A100Types::typeX A100_GAUSSIAN_NOISE_MEAN;
extern const A100Types::typeX A100_GAUSSIAN_NOISE_STDDEV;


class A100 : public ControlLoop<A100Types::typeA, A100Types::typeB,
                                            A100Types::typeX, A100Types::typeC,
                                            A100Types::typeY, A100Types::typeU,
                                            A100Types::typeK, A100Types::typeL,
                                            A100Types::typeR>
{
public:
    A100();
};

#endif // A100_H
