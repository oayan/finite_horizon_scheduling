/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef CONTROLAPPLICATION_H
#define CONTROLAPPLICATION_H

#include "NetworkBase/mobileterminal.h"
#include "NetworkBase/application.h"

#include "Eigen/Core"
using namespace  Eigen;

#include <iostream>
#include <unsupported/Eigen/MatrixFunctions>

class ControlApplication : public Application
{

    struct ControlSignal
    {
        VectorXd u;
        int timeStep;
    };

public:
    ControlApplication(MobileTerminal * const tx, MobileTerminal * const rx);
    ~ControlApplication();

    template <typename typeA, typename typeB, typename typeL>
    void initControl(const MatrixBase<typeA> & _A, const  MatrixBase<typeB> &_B, const  MatrixBase<typeL> &_L, const  MatrixBase<typeA> &_W, const int & ts_ms, const int & t);

    bool updateBuffer(const packet & newPkt);

    MatrixXd A;
    MatrixXd B;
    MatrixXd L;
    MatrixXd W;

    /**
     * @brief getApower Matrix power calculation
     * @param pow integer power of a matrix
     * @return A^pow
     */
    MatrixXd getApower(int pow);

    /**
     * @brief get_aoi Getter function for Age-of-Information
     * @return difference between current time step (k) and timestep of information available at BS (m)
     */
    int getAoi() const;

    double getULUtility() const;

    void setULUtility(double util);

    bool timePass_1ms(int t); //returns true if plant is sampled

    void mimicSampling();

    int getSamplingOffset() const;

    //private:
    VectorXd x_m;

    int l;

    int m;

    int k;

    int t_until_next_sample_ms;

    int sampling_period_ms;

    int samplingOffset_ms;

    int AoI;

    double ULutility;
    int getAgeInXSlots(int x);
};

template <typename typeA, typename typeB, typename typeL>
void ControlApplication::initControl(const MatrixBase<typeA> & _A, const  MatrixBase<typeB> & _B, const  MatrixBase<typeL> & _L, const MatrixBase<typeA> & _W, const int & ts_ms, const int & samplingOffset)
{
    A = _A;
    B = _B;
    L = _L;
    W = _W;

    x_m = VectorXd::Zero(A.rows());

    sampling_period_ms = ts_ms;
    samplingOffset_ms = samplingOffset;



    t_until_next_sample_ms = sampling_period_ms + samplingOffset_ms;
    //qDebug() << "ControlApplication: initControl(): t_until_next_sample" << t_until_next_sample_ms;
}





#endif // CONTROLAPPLICATION_H
