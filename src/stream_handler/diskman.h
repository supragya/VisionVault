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

    void DiskMan(rBuf *globalBuffer, char *buf, long *offset);

    int DiskAppend(char *data, long len, char *buf, long *offset);

    void DiskManagerThreadEntry(RawStreamHandler::rBuf *);
}

#endif