//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the stream handler for RAW frame inputs

#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

namespace RawStreamHandler::fm {
    void Write2Buffer(uint8_t **buf);

    void ReadFromStream();

    void ThreadEntry();
}

#endif