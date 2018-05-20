//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#include "frameman.h"
#include "buffers.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace RawStreamHandler;

void FrameManThreadEntry(rBuf globalBuffer) {
    const char *frameLoc = "";
    axiomFrames frameBlock;

    std::ifstream fFile(frameLoc);

    if (!fFile.is_open()) {
        std::cout << "Meta Stream unavailable. Exiting" << std::endl;
        exit(0);
    }

    int ctr = 0;
    auto start = std::chrono::system_clock::now();
    while (!fFile.eof()) {
        fFile.read(reinterpret_cast<char *>(&frameBlock), sizeof(frameBlock));
        globalBuffer.pushData(MetaBuf, reinterpret_cast<uint8_t *>(&frameBlock), sizeof(frameBlock));
        ctr++;
    }
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_second_ = end - start;

    double elapsed_second = elapsed_second_.count() / ctr;

    std::cout << "Frame time per frame: " << elapsed_second << ", max: " << 1 / elapsed_second << " per second"
              << std::endl;
}