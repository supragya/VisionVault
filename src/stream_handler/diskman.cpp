//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#include "diskman.h"
#include <memory.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

using namespace RawStreamHandler;

void RawStreamHandler::DiskMan(RawStreamHandler::rBuf *globalBuffer, char *buf, long *offset) {
    while (true) {
        // Check if frameBuf needs a dump

        for (int i = 0; i < 2; i++) {
            if (globalBuffer->frameFlushable[i]) {
                // Lock mutex
                globalBuffer->frameMutex[i].lock();

                if (DiskAppend(globalBuffer->frameBuf[i], globalBuffer->frameOffset[i], buf, offset)) {
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

                if (DiskAppend(globalBuffer->metaBuf[i], globalBuffer->metaOffset[i], buf, offset)) {
                    globalBuffer->metaOffset[i] = 0;
                    globalBuffer->metaFlushable[i] = false;
                }

                // Unlock mutex
                globalBuffer->metaMutex[i].unlock();
            }
        }

    }
}

int RawStreamHandler::DiskAppend(char *data, long len, char *buf, long *offset) {
    memcpy(reinterpret_cast<void *>(buf + *offset), data, len);
    *offset += len;
    return 0;
}

void RawStreamHandler::DiskManagerThreadEntry(RawStreamHandler::rBuf *globalBuffer) {
    char *writeSpace = new char[3l * 1024 * 1024 * 1024]; // 3GB ;
    long writeOffset = 0;
    DiskMan(globalBuffer, writeSpace, &writeOffset);
}
