//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#include "diskman.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void RawStreamHandler::DiskMan(RawStreamHandler::rBuf *globalBuffer, std::ofstream *cacheFile) {
    while (true) {
        // Check if frameBuf needs a dump

        for (int i = 0; i < 2; i++) {
            if (globalBuffer->frameFlushable[i]) {
                // Lock mutex
                globalBuffer->frameMutex[i].lock();

                if (DiskAppend(globalBuffer->frameBuf[i], globalBuffer->frameOffset[i], cacheFile)) {
                    globalBuffer->frameOffset[i] = 0;
                    globalBuffer->frameFlushable[i] = false;
                }

                // Unlock mutex
                globalBuffer->frameMutex[i].unlock();
            }
        }

        // Check if metaBuf needs a dump

        for (int i = 0; i < 2; i++) {
            if (globalBuffer->metaFlushable[i]) {
                // Lock mutex
                globalBuffer->metaMutex[i].lock();

                if (DiskAppend(globalBuffer->metaBuf[i], globalBuffer->metaOffset[i], cacheFile)) {
                    globalBuffer->metaOffset[i] = 0;
                    globalBuffer->metaFlushable[i] = false;
                }

                // Unlock mutex
                globalBuffer->metaMutex[i].unlock();
            }
        }

    }
}

int RawStreamHandler::DiskAppend(char *data, long len, std::ofstream *cacheFile) {
    std::cout << "DiskAppend called with tellp: " << cacheFile->tellp() << std::endl;
    cacheFile->write(data, len);
    return 0;
}

void RawStreamHandler::DiskManagerThreadEntry(RawStreamHandler::rBuf *globalBuffer) {
    std::ofstream diskCache;
    diskCache.open("cache.mlv", std::ios::trunc);
    if (!diskCache) {
        std::cout << "Error while opening cache.mlv, exiting the application." << std::endl;
        exit(1);
    }
    DiskMan(globalBuffer, &diskCache);
}
