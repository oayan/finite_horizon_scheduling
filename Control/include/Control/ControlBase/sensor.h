/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef SENSOR_H
#define SENSOR_H

#include "Common/block.h"


template <typename typeX, typename typeC, typename typeY>
class Sensor : public Block<typeX, typeY>
{
public:

    /**
     * @brief Sensor Sensor class constructor
     * @param _C Output matrix C
     */
    Sensor(const typeC& _C);


    void write(const typeY &v, const int ts);
    void update();

    /**
     * @brief update Reads the incoming link from plant, i.e. samples the plant, and forwards it to the outgoing link
     */
    void sample(const int k);

private:
    typeY y;
    typeC C;

};

template<typename typeX, typename typeC, typename typeY>
Sensor<typeX, typeC, typeY>::Sensor(const typeC &_C)
    : Block<typeX, typeY>()
{
    C = _C;
    y.setZero();
}

template<typename typeX, typename typeC, typename typeY>
void Sensor<typeX, typeC, typeY>::write(const typeY& v, const int ts)
{
    this->outputLink->setValue(v);
    this->outputLink->setTimeStamp(ts);
}

template<typename typeX, typename typeC, typename typeY>
void Sensor<typeX, typeC, typeY>::update()
{
    typeX x = this->read();
    y = C * x;
}

template<typename typeX, typename typeC, typename typeY>
void Sensor<typeX, typeC, typeY>::sample(const int k)
{
    update();
    this->write(y, k);
}


#endif // SENSOR_H
