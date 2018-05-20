//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the disk management methods (flush to disk operations)

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "buffers.h"
#include <iostream>

namespace RawStreamHandler {

    uint8_t *writeSpace;
    long writeOffset;

    void DiskMan(rBuf globalBuffer);

    int DiskAppend(char *data, long len);

    void DiskManagerThreadEntry(long writeLenTemp);
}

#endif