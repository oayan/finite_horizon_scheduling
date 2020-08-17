/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Common/block.h"
#include "estimator.h"
#include <iostream>

template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
class Controller : public Block<typeY, typeU>
{
public:
    /**
        @brief Controller class constructor
        @param _ref desired reference value
        @param _K input gain matrix
        @param _L feedback gain matrix
        @param ts_msec sampling period in ms
    */
    Controller(const typeR& _ref, const typeK& _K, const typeL& _L, unsigned short _ts_msec);

    /**
     * @brief Controller class destructor
     */
    ~Controller();


    std::tuple<typeY, int> read();

    /**
     * @brief initEstimator
     * @param _A System matrix of the controlled plant
     * @param _B Input matrix of the controlled plant
     * @param _L Feedback gain matrix of the controlled plant
     */
    void initEstimator(const typeA& _A, const typeB& _B, const typeL& _L);

    /**
     * @brief control Calculates control input u and writes to output link
     * @param _k Time step k of the plant
     */
    void control(const unsigned short& _k);

    typeX getEstimate();

    typeL getLMatrix();

    void measureAoI();

    double getAverageAoI(const int totalSimulationTime_ms) const;

    int getAOI() const;

    //protected:
    /**
     * @brief update Reads input link and updates the corresponding control signal u internally
     */
    void update();
    const typeK K;
    const typeL L;
    const typeR ref;
    typeU u;
    const unsigned short ts_msec;
    unsigned int k;
    Estimator<typeY, typeA, typeB, typeL, typeX, typeU>* estimator;
    double totalAoI;
    typeU getControlInput();
};


template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::Controller(const typeR& _ref, const typeK& _K, const typeL& _L, unsigned short _ts_msec)
    : Block<typeY, typeU>()
    , K(_K)
    , L(_L)
    , ref(_ref)
    , ts_msec(_ts_msec)
{
    u.setZero();
    k = 0;
    totalAoI = 0.0;
}


template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::~Controller()
{
    delete estimator;
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
std::tuple<typeY, int> Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::read()
{
    return std::make_tuple(this->inputLink->readValue(), this->inputLink->readTimeStamp());
}


template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
void Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::initEstimator(const typeA &_A, const typeB &_B, const typeL &_L)
{
    estimator = new Estimator<typeY, typeA, typeB, typeL, typeX, typeU>(_A, _B, _L);
}


template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
void Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::control(const unsigned short & _k)
{
    k = _k;
    assert(k == estimator->k);
    this->update();
    this->write(u);
    //    qDebug() << "Controller: write: " << u[0];
    estimator->stepNext();
}


template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
typeX Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::getEstimate()
{
    return estimator->getEstimate();
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
typeU Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::getControlInput()
{
    return u;
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
typeL Controller<typeY, typeA, typeX, typeB, typeK, typeL, typeR, typeU>::getLMatrix()
{
    return L;
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
void Controller<typeY, typeA, typeX, typeB, typeK, typeL, typeR, typeU>::measureAoI()
{
    totalAoI += static_cast<double>(estimator->getAOI());
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
double Controller<typeY, typeA, typeX, typeB, typeK, typeL, typeR, typeU>::getAverageAoI(const int totalSimulationTime_ms) const
{
    return totalAoI / static_cast<double>(totalSimulationTime_ms);
}

template<typename typeY, typename typeA, typename typeX, typename typeB, typename typeK, typename typeL, typename typeR, typename typeU>
int Controller<typeY, typeA, typeX, typeB, typeK, typeL, typeR, typeU>::getAOI() const
{
    return estimator->getAOI();
}


template <typename typeY, typename typeA, typename typeX, typename typeB,  typename typeK, typename typeL, typename typeR, typename typeU>
void Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>::update()
{
    auto t = this->read();
    typeY y = std::get<0>(t);
    int l = std::get<1>(t);
    // qDebug() << "Controller::update(): packet read l = " << l;
    // For now perfect observation
    typeX x = y;

    estimator->update(x, l);
    typeX x_hat_c = estimator->getEstimate();
    u = (ref * K) - (L * x_hat_c);
}

#endif // CONTROLLER_H
