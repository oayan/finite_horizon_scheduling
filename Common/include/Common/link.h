/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LINK_H
#define LINK_H

template <typename T>
class Link{ 

public:
    /**
     * @brief Link Link class constructor
     */
    Link();

    /**
     * @brief readValue Getter function for link's value. Called by block(s) which has this link as input
     * @return
     */
    T readValue();

    /**
     * @brief setValue Sets given value to link. Called by block(s) which has this link as output link
     * @param v value to set
     */
    void setValue(const T v);

    /**
     * @brief readTimeStamp Link might be set with a timestamp based on the writer or reader block.
     * This reads the timestamp attribute of the link value.
     * @return timestamp value of the link value
     */
    int readTimeStamp();

    /**
     * @brief setTimeStamp Sets the timestamp value of the link
     * @param ts timestamp value to set
     */
    void setTimeStamp(int ts);

private:
    T value;
    int timeStamp;
};

template<typename T>
Link<T>::Link()
{
    value.setZero();
    timeStamp = -1;
}

template<typename T>
T Link<T>::readValue()
{
    return value;
}

template<typename T>
void Link<T>::setValue(const T v) {
    value = v;
}

template<typename T>
int Link<T>::readTimeStamp()
{
    return timeStamp;
}

template<typename T>
void Link<T>::setTimeStamp(int ts)
{
    timeStamp = ts;
}

#endif // LINK_H
