/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef PACKET_H
#define PACKET_H
#include <cstring>

#define PACKET_SIZE_IN_REAL_BYTES 32.0
#define MAX_DATA_LEN 6

struct packet
{
  int dataLen;
  double data[MAX_DATA_LEN];
  int timeStamp;
  int destinationID;
};

#endif // PACKET_H
