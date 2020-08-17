/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef GAUSSIANNOISE_H
#define GAUSSIANNOISE_H

#include "noise.h"
#include <random>

template <typename T>
class GaussianNoise : virtual public Noise
{
public:
    static T generateNoise(unsigned seed, const T& mean, const T& stddev)
    {
        T noiseVec(mean.rows());
        std::default_random_engine generator(seed);

        for(int i = 0; i < noiseVec.rows(); i++)
        {
            std::normal_distribution<double> distribution(mean(i), stddev(i));
            noiseVec(i) = distribution(generator);
        }

        return noiseVec;
    }
};

#endif // GAUSSIANNOISE_H
