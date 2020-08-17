/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BLOCK_H
#define BLOCK_H

#include "link.h"

template <typename typeIn, typename typeOut>
class Block
{
public:
    /**
     * @brief Block Block class constructor
     */
    Block();

    /**
     * @brief setInputLink assign an input link to block
     * @param l constant pointer to link instance
     */
    void setInputLink(Link<typeIn>* const l);

    /**
     * @brief setOutLink assign an output link to block
     * @param l constant pouinter to link instance
     */
    void setOutLink(Link<typeOut>* const l);

    /**
     * @brief update virtual method. Derived class overrides this method
     */
    virtual void update() = 0;
protected:
    virtual ~Block() {}
    Link<typeIn>* inputLink;
    Link<typeOut>* outputLink;

    /**
     * @brief read Getter method for reading the input link
     * @return Returns input link value
     */
    typeIn read();

    /**
     * @brief write Setter method for writing to the output link
     * @param v Value to write
     */
    void write(const typeOut& v);
};

template<typename typeIn, typename typeOut>
Block<typeIn, typeOut>::Block()
{

}

template<typename typeIn, typename typeOut>
void Block<typeIn, typeOut>::setInputLink(Link<typeIn> * const l)
{
    inputLink = l;
}

template<typename typeIn, typename typeOut>
void Block<typeIn, typeOut>::setOutLink(Link<typeOut> * const l)
{
    outputLink = l;
}

template<typename typeIn, typename typeOut>
typeIn Block<typeIn, typeOut>::read()
{
    return inputLink->readValue();
}

template<typename typeIn, typename typeOut>
void Block<typeIn, typeOut>::write(const typeOut& v)
{
    outputLink->setValue(v);
}

#endif // BLOCK_H
