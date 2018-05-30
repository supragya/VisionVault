//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the disk management methods (flush to disk operations)

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include "buffers.h"
#include <iostream>
#include <fstream>

namespace RawStreamHandler {

    void DiskMan(rBuf *globalBuffer, std::ofstream *cacheFile);

    int DiskAppend(char *data, long len, std::ofstream *cacheFile);

    void DiskManagerThreadEntry(RawStreamHandler::rBuf *);
}

#endif