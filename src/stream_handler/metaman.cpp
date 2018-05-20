//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//


#include "metaman.h"
#include "buffers.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace RawStreamHandler;

void MetaManThreadEntry(rBuf globalBuffer) {
    const char *metaLoc = "";
    axiomMeta metaBlock;

    std::ifstream mFile(metaLoc);

    if (!mFile.is_open()) {
        std::cout << "Meta Stream unavailable. Exiting" << std::endl;
        exit(0);
    }

    int ctr = 0;
    auto start = std::chrono::system_clock::now();
    while (!mFile.eof()) {
        mFile.read(reinterpret_cast<char *>(&metaBlock), sizeof(metaBlock));
        globalBuffer.pushData(MetaBuf, reinterpret_cast<uint8_t *>(&metaBlock), sizeof(metaBlock));
        ctr++;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_second_ = end - start;

    double elapsed_second = elapsed_second_.count() / ctr;

    std::cout << "Meta time per frame: " << elapsed_second << ", max: " << 1 / elapsed_second << " per second"
              << std::endl;
}