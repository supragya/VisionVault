//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the stream handler for Metadata

#ifndef METAMANAGER_H
#define METAMANAGER_H

namespace RawStreamHandler::mm {
    void Write2Buffer(uint8_t **buf);

    void ReadFromStream();

    void ThreadEntry();
}


#endif