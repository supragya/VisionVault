//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the stream handler for RAW frame inputs

#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <cstdint>
#include "../common/mlv.h"
#include "buffers.h"
#include <fstream>

namespace RawStreamHandler {

    struct axiomFrames {
        mlv_vidf_hdr_t vidf;
        uint8_t reserved[4]; // Other things that come along, eg. markers
    };

    void FrameManThreadEntry(RawStreamHandler::rBuf *);
}

#endif