/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef CONTROLLOOP_H
#define CONTROLLOOP_H
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "plant.h"
#include "controller.h"
#include "sensor.h"
#include "Common/link.h"
#include "Network/txgateway.h"
#include "Network/rxgateway.h"
#include "Network/basestation.h"

template <typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
class ControlLoop
{

public:
    /**
     * @brief ControlLoop ControlLoop class constructor
     * @param _A System matrix
     * @param _B Input matrix
     * @param _x Initial state vector
     * @param _C Output matrix
     * @param _K Input gain matrix
     * @param _L Feedback gain matrix
     * @param _ref desired reference value for control
     * @param _ts_msec Sampling period in milliseconds
     */
    ControlLoop(const typeA& _A,   const typeB& _B,
                const typeX& _x,   const typeC& _C,
                const typeK& _K,   const typeL& _L,
                const  typeR& _ref, const unsigned short _ts_msec,
                typeX noiseMean, typeX noiseStdDev);

    int getRcvdMsgCnt(){return rxGw->rcvdMsgCntr;}

    int getTxedMsgCnt(){return txGw->txedMsgCntr;}

    int getScheduledCnt(){return txGw->scheduledCntr;}

    double getAverageDelay();

    void startMeasuring();
    /**
     * @brief ~ControlLoop ControlLoop class destructor
     */
    ~ControlLoop()
    {
        delete plant;
        delete controller;
        delete sensor;
        delete plantToSensorLink;
        delete controllerToPlantLink;
        delete sensorToGwLink;
        delete gwToControllerLink;
        delete txGw;
        delete rxGw;
    }

    /**
     * @brief sample First makes plant write state to P-To-S link and afterwards makes sensor forward information from P-To-S link to S-To-C link
     */
    void sample();

    /**
     * @brief control Makes controller read the input link and write the newly calculated control input to C-To-P link
     */
    void control();

    /**
     * @brief actuate Makes plant to read from S-To-P link and update its state and time step
     */
    void actuate() const;

    /**
     * @brief update Calls sample, control and actuate methods in order
     */
    void update();

    void initNetwork(BaseStation * const bs);

    /**
     * @brief getPlant Returns pointer to loop's plant object
     * @return Pointer to const plant object
     */
    const Plant<typeU, typeA, typeB, typeX>* getPlant() const;

    /**
     * @brief getController Returns pointer to loop's controller object
     * @return Pinter to const plant object
     */
    const Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>* getController() const;

    double getAverageAoI() const;

    double getIAE() const;

    double getINIE() const;

    double getJ() const;

    double getAverageNIE() const;

    int getSampledMsgCnt() const;

    double getAverageJ() const;

    //protected:
    Link<typeX>* plantToSensorLink;
    Link<typeU>* controllerToPlantLink;
    Link<typeY>* sensorToGwLink;
    Link<typeY>* gwToControllerLink;
    TxGateway<typeY>* txGw;
    RxGateway<typeY>* rxGw;
    unsigned short ts_msec;
    unsigned int total_sim_time_ms;
    int t_until_next_sample_ms;
    int samplingOffset;

    void registerToBs(BaseStation * const bs);
    BaseStation * bs;

    double integratedNIE;

    double integratedJ;

    //private:
    Plant       <typeU, typeA, typeB, typeX>* plant;
    Controller  <typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>* controller;
    Sensor      <typeX, typeC, typeY>* sensor;
    unsigned int k;
    unsigned int measured_k;
    /**
     * @brief connectBlocks assigns links to blocks accordingly
     */
    void connectBlocks() const;
    bool debug_flag;

    int sampledMessageCnt;
    bool recordStats;
};

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::ControlLoop(const typeA& _A,   const typeB &_B,
                                                                                        const typeX& _x,   const typeC &_C,
                                                                                        const typeK& _K,   const typeL &_L,
                                                                                        const typeR &_ref, const unsigned short _ts_msec,
                                                                                        typeX noiseMean, typeX noiseStdDev)
{
    plant = new Plant<typeU, typeA, typeB, typeX>(_A, _B, _x, noiseMean, noiseStdDev);

    controller = new Controller<typeY, typeA, typeX, typeB,  typeK, typeL, typeR, typeU>(_ref, _K, _L, _ts_msec);
    controller->initEstimator(_A, _B, _L);

    sensor = new Sensor<typeX, typeC, typeY>(_C);
    ts_msec = _ts_msec;
    plantToSensorLink = new Link<typeX>;
    controllerToPlantLink = new Link<typeU>;
    sensorToGwLink = new Link<typeY>;
    gwToControllerLink = new Link<typeY>;
    connectBlocks();

    /* initialize random seed: */
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    int r = rand();
    samplingOffset = r % ts_msec; // [0, T_s)
    t_until_next_sample_ms = samplingOffset + ts_msec;
    // qDebug() << "ControlLoop(): samplingOffset: " << samplingOffset;
    k = 0;
    measured_k = 0;
    integratedNIE = 0.0;
    integratedJ = 0.0;
    debug_flag = false;
    sampledMessageCnt = 0;

    total_sim_time_ms = 0;

    recordStats = false;

}


template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getAverageDelay()
{
    if(rxGw->rcvdMsgCntr)
        return (rxGw->totalDelay / static_cast<double>(rxGw->rcvdMsgCntr));
    else return 0;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::startMeasuring()
{
    recordStats = true;
    rxGw->startMeasuring();
    txGw->startMeasuring();
}


template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::sample()
{
    plant->getSampled();
    sensor->sample(k);

    txGw->recvFromPlant();

    if(recordStats)
        sampledMessageCnt++;
    return;

}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::control()
{
    controller->control(k);
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::actuate() const
{
    plant->getControlled();
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::update()
{
    if(t_until_next_sample_ms == 0)
    {
        rxGw->receiveFromBS(k);
        control();
    }

    if(recordStats) {
        controller->measureAoI();
        total_sim_time_ms ++;
        typeX nie = plant->getState() - controller->getEstimate();
        measured_k++;
        integratedNIE += (nie.transpose() * nie).norm();
        if(plant->getState().rows() > 1)
            integratedJ += (plant->getState().transpose() * plant->getState() /*+ controller->getControlInput().transpose() * controller->getControlInput()*/).norm();
        else
            integratedJ += pow(plant->getState().norm(), 2) /*+ pow(controller->getControlInput().norm(), 2)*/;
    }

    // qDebug() << "ControlLoop: t_until_next_sample_ms: " << t_until_next_sample_ms;
    if(t_until_next_sample_ms-- == 0)
    {
        t_until_next_sample_ms = ts_msec - 1;

        // Controller: Uses only what has been received until now

        actuate();
        k++;
        sample();
    }
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::connectBlocks() const
{
    plant->setInputLink(controllerToPlantLink);
    plant->setOutLink(plantToSensorLink);
    sensor->setInputLink(plantToSensorLink);
    sensor->setOutLink(sensorToGwLink);
    controller->setInputLink(gwToControllerLink);
    controller->setOutLink(controllerToPlantLink);
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
const Plant<typeU, typeA, typeB, typeX>* ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getPlant() const
{
    return plant;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
const Controller<typeY, typeA, typeX, typeB, typeK, typeL, typeR, typeU>* ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getController() const
{
    return controller;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getAverageAoI() const
{
    return controller->getAverageAoI(total_sim_time_ms);
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getIAE() const
{
    return plant->getIAE();
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getINIE() const
{
    return integratedNIE;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getJ() const
{
    return integratedJ;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getAverageNIE() const
{
    return getINIE() / measured_k;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
double ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getAverageJ() const
{
    return getJ() / measured_k;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
int ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::getSampledMsgCnt() const
{
    return sampledMessageCnt;
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::initNetwork(BaseStation * const bs)
{
    this->bs = bs;
    txGw = new TxGateway<typeY>();
    rxGw = new RxGateway<typeY>();
    txGw->setInputLink(sensorToGwLink);
    rxGw->setOutLink(gwToControllerLink);
    registerToBs(bs);
    txGw->setPartnerID(rxGw->getID());
    rxGw->setPartnerID(txGw->getID());
    txGw->transmitter = true;
    rxGw->transmitter = false;
    sample();
}

template<typename typeA, typename typeB, typename typeX, typename typeC, typename typeY, typename typeU, typename typeK, typename typeL, typename typeR>
void ControlLoop<typeA, typeB, typeX, typeC, typeY, typeU, typeK, typeL, typeR>::registerToBs(BaseStation * const bs)
{
    bs->registerLoop((MobileTerminal*) txGw, (MobileTerminal*) rxGw, plant->getAMatrix(), plant->getBMatrix(), controller->getLMatrix(), plant->getWMatrix(), ts_msec, samplingOffset);

}

#endif // CONTROLLOOP_H
