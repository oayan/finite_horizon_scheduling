/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <unsupported/Eigen/MatrixFunctions>
#include <iostream>
#include <vector>

template <typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
class Estimator
{
    struct Estimation {
        typeX estimate;
        int timestamp;
    };

    struct ControlInput {
        typeU input;
        int timestamp;
    };

public:
    /**
     * @brief Estimator Estimator class contsructor
     * @param _A System matrix of the estimated plant
     * @param _B Input matrix of the estimated plant
     * @param _L Feedback gain matrix of the estimated plant
     */
    Estimator(const typeA & _A, const typeB & _B, const typeL & _L);

    /**
     * @brief getEstimate Getter function for the current estimation
     * @return estimated plant state
     */
    typeX getEstimate() const;

    /**
     * @brief stepNext make estimation move to one discrete step in time
     */
    void stepNext();

    /**
     * @brief update Updates the estimation x_k for current time-step k given sensor vector y_l belonging to time-step l
     * i.e., (E[x[k] | y[l] = y_l]
     * @param l time-step of the output vector
     * @param y_l output vector belonging to time-step l
     */
    void update(const typeY & _x_l, const int l);

    int getAOI() const;

//private:
    const typeA A;          // Time invariant system matrix A
    const typeB B;          // Time invariant input matrix B
    const typeL L;          // Time invariant feedback gain matrix L
    int k;                  // Current time-step k
    int l;                  // Time-step l, to which the most recent information belongs to
    typeX x_hat_k;          // Current estimate E[x_hat_k]
    typeX x_hat_l;          // Estimate at time-step l
    typeX x_l;              // Latest received true state x[l] belonging to time-step l

    const typeA BL;         // BL = B*L
    const typeA A_BL;         // A_BL = A - B * L
    Eigen::MatrixPower<typeA> Apow;
    Eigen::MatrixPower<typeA> A_BLpow;
    std::vector<ControlInput> U;

    /**
     * @brief getApower Matrix power calculation
     * @param pow integer power of a matrix
     * @return A^pow
     */
    typeA getApower(int pow);

    /**
     * @brief Setter function for current estimate x_hat_k
     * @param new estimate value
     */
    void setEstimate(const typeX& new_est);

};


template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::Estimator(const typeA & _A, const typeB & _B, const typeL & _L)
    : A(_A)
    , B(_B)
    , L(_L)
    , BL(B * L)
    , A_BL(A - BL)
    , Apow(A)
    , A_BLpow(A_BL)
{
    x_hat_k.setZero();
    x_hat_l.setZero();
    k = 0;
    l = -1;
    U.clear();
}


template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
typeX Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::getEstimate() const
{
    return x_hat_k;
}


template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
void Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::stepNext()
{
    ControlInput cIn;
    cIn.input = - (L * x_hat_k);
    cIn.timestamp = k;

    U.push_back(cIn);
    this->k += 1;
}


template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
void Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::update(const typeY & _x_l, const int _l)
{
    if (_l < 0)
        return;

    //No change since last update
    if(_l <= l)
    {
        typeX sum_term = typeX::Zero();
        for(int q = 1; q <= k - l; q++)
            sum_term = sum_term + (getApower(q - 1) * B * U.at(U.size() - q).input);
        x_hat_k = (getApower(k - l) * x_l) + sum_term;
        return;
    }

    // Change since last update (new information)
    x_l = _x_l;
    l = _l;

    if(k == l)
    {
        // New and non-delayed information (best case)
        x_hat_k = x_l;
        U.clear();
    }
    else
    {
        // Delayed information (l < k)
        // Delete entries from history which are older than l
        while(!U.empty() && U.front().timestamp < l)
            U.erase(U.begin());

        // New information but delayed (l < k)
        int q = 1;
        typeX backwards_correction_term = typeX::Zero();
        // TODO update the equation with ref and K matrices
        do {
            backwards_correction_term = backwards_correction_term + (Apow(q - 1) * B * U.at(U.size() - q).input);
        } while(++q <= (k - l));
        x_hat_k = Apow(k - l) * x_l + backwards_correction_term;
        x_hat_l = x_hat_k;
    }

}

template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
int Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::getAOI() const
{
    return k-l;
}

template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
typeA Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::getApower(int pow)
{
    Eigen::MatrixPower<typeA>Apow(A);
    return Apow(pow);
}


template<typename typeY, typename typeA, typename typeB, typename typeL, typename typeX, typename typeU>
void Estimator<typeY, typeA, typeB, typeL, typeX, typeU>::setEstimate(const typeX & new_est)
{
    this->x_hat_k = new_est;
}



#endif // ESTIMATOR_H
