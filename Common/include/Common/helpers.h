/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <iostream>
#include <fstream>

template <typename T>
int recordStateToFile(const std::string & fileName, const int & k, const T & stateToRecord) {
    std::ofstream out_file(fileName, std::ios::out | std::ios::app);
    if(!out_file.is_open())
        return 1;
    out_file << k;
    for(int r = 0; r < T::RowsAtCompileTime; r++)
    {
        out_file << "," << stateToRecord(r);
    }
    out_file << std::endl;
    return 0;
}

extern double dB_to_linear(double dB);

extern double linear_to_dB(double linear);

extern double mW_to_dBm(double mW);

extern double dBm_to_mW(double dBm);

extern double Q(double x);

#endif // HELPERS_H
