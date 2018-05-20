//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the stream handler for Metadata

#ifndef METAMANAGER_H
#define METAMANAGER_H

#include <iostream>
#include "../common/mlv.h"
#include "buffers.h"

namespace RawStreamHandler {

    struct axiomMeta {
        mlv_expo_hdr_t expo;
        mlv_lens_hdr_t lens;
    };

    void MetaManThreadEntry(RawStreamHandler::rBuf *);
}


#endif