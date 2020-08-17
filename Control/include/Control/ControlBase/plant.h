/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef PLANT_H
#define PLANT_H

#include "Common/block.h"
#include "Common/gaussiannoise.h"
#include <chrono>
#include <iostream>
#include <tuple>
#define MAX_STATE  1000000.0
#define MIN_STATE -1000000.0
//#undef MAX_STATE
//#undef MIN_STATE

template <typename typeU, typename typeA, typename typeB, typename typeX>
class Plant : public Block<typeU,typeX>
{

public:
    /**
     * @brief Plant Plant class constructor
     * @param _A System matrix
     * @param _B Input matrix
     * @param _x Initial state vector
     */
    Plant(const typeA& _A, const typeB& _B, const typeX& _x, const typeX& _noiseMean, const typeX& _noiseStdDev);

    /**
     * @brief getAMatrix Getter method for the system matrix
     * @return System matrix, A
     */
    typeA getAMatrix() const;

    /**
     * @brief getBMatrix Getter method for the input matrix
     * @return Input matrix, B
     */
    typeB getBMatrix() const;

    typeA getWMatrix() const;

    const typeX& getState() const;

    /**
     * @brief getSampled Get the plant sampled by making it write to output
     */
    void getSampled();

    /**
     * @brief getControlled Get the plant controlled by reading the input link applying it to itself
     */

    void getControlled();

    /**
     * @brief update Update method for the plant block to process input, move one step in time and write to output
     */
    void update();

    /**
     * @brief stepNext Make plant move one step in time
     */
    void stepNext(const typeU& u);

    std::vector<std::tuple<int, typeX> > getStateRecords() const;

    double getIAE() const;

protected:
    const typeA A;              // Time invariant system matrix A
    const typeB B;              // Time invariant input matrix B
    const typeX noiseMean;
    const typeX noiseStdDev;
    typeA W;                    // Noise covariance matrix W
    typeX x;                    // Plant state vector
    int k;
    double IAEnorm;
};


template<typename typeU, typename typeA, typename typeB, typename typeX>
Plant<typeU, typeA, typeB, typeX>::Plant(const typeA& _A, const typeB& _B, const typeX& _x,
                                         const typeX &_noiseMean, const typeX &_noiseStdDev)
    : Block<typeU,typeX>()
    , A(_A)
    , B(_B)
    , noiseMean(_noiseMean)
    , noiseStdDev(_noiseStdDev)
    , W((noiseStdDev.cwiseProduct(noiseStdDev)).asDiagonal())
{
    x = _x + GaussianNoise<typeX>::generateNoise(std::chrono::system_clock::now().time_since_epoch().count(), noiseMean, noiseStdDev);;
    k = 0;
    IAEnorm = 0.0;
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
typeA Plant<typeU, typeA, typeB, typeX>::getAMatrix() const {
    return A;
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
typeB Plant<typeU, typeA, typeB, typeX>::getBMatrix() const {
    return B;
}

template<typename typeU, typename typeA, typename typeB, typename typeX>
typeA Plant<typeU, typeA, typeB, typeX>::getWMatrix() const
{
    return W;
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
const typeX& Plant<typeU, typeA, typeB, typeX>::getState() const
{
    return x;
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
void Plant<typeU, typeA, typeB, typeX>::getSampled()
{
    this->write(x);
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
void Plant<typeU, typeA, typeB, typeX>::getControlled()
{
    this->update();
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
void Plant<typeU, typeA, typeB, typeX>::stepNext(const typeU& u)
{
    typeX w = GaussianNoise<typeX>::generateNoise(std::chrono::system_clock::now().time_since_epoch().count(), noiseMean, noiseStdDev);

    x = (A * x) + (B * u) + w;

#ifdef MAX_STATE
    if(x(0,0) > MAX_STATE){
        x(0,0) = MAX_STATE;
    }
    else if(x(0,0) < MIN_STATE){
        x(0,0) = MIN_STATE;
    }
#endif
    k++;
}

template<typename typeU, typename typeA, typename typeB, typename typeX>
double Plant<typeU, typeA, typeB, typeX>::getIAE() const
{
    return IAEnorm / (double) k;
}


template<typename typeU, typename typeA, typename typeB, typename typeX>
void Plant<typeU, typeA, typeB, typeX>::update() {
    typeU u = this->read();
    this->stepNext(u);
    IAEnorm += x.norm();
}

#endif // PLANT_H
