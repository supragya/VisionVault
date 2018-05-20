//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the disk management methods (flush to disk operations)

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <iostream>

namespace RawStreamHandler::dm {

    uint8_t *writeSpace;

    void DiskMan();

    int DiskAppend(uint8_t *data, long len);

    void ThreadEntry(long writeLenTemp);
}

#endif