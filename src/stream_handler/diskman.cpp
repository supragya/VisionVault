//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#include "diskman.h"
#include "buffers.h"

using namespace RawStreamHandler;

void dm::DiskMan() {
    while (true) {
        // Check if frameBuf needs a dump

        for (int i = 0; i < 2; i++) {
            if (rBuf.frameFlushable[i]) {
                // Lock mutex
                rBuf.frameMutex[i].lock();

                if (dm::DiskAppend(rBuf.frameBuf[i], frameOffset[i])) {
                    rBuf.frameOffset[i] = 0;
                    rBuf.frameFlushable[i] = false;
                }

                // Unlock mutex
                rBuf.frameMutex[i].unlock();
            }
        }

        // Check if metaBuf needs a dump

        for (int i = 0; i < 2; i++) {
            if (rBuf.metaFlushable[i]) {
                // Lock mutex
                rBuf.metaMutex[i].lock();

                if (dm::DiskAppend(rBuf.metaBuf[i], metaOffset[i])) {
                    rBuf.metaOffset[i] = 0;
                    rBuf.metaFlushable[i] = false;
                }

                // Unlock mutex
                rBuf.metaMutex[i].unlock();
            }
        }

    }
}

void dm::DiskAppend(uint8_t *data, long len) {

}