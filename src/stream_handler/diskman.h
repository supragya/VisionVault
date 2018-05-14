//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the disk management methods (flush to disk operations)

#ifndef DISKMANAGER_H
#define DISKMANAGER_H

#include <iostream>

namespace RawStreamHandler::dm {
    const long BUFSIZE = 512 * 1024 * 1024;
    struct buffer {
        uint8_t *buf;
        std::mutex bufmtx;
        long bufptr;
    };

    buffer framebuf[2];
    buffer metabuf[2];

    int Flush2Disk(uint8_t *buf);

    void ThreadEntry();
}

#endif