//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// TODO: Simplify

#include "buffers.h"
#include <thread>

using namespace RawStreamHandler;

int rBuf::pushData(RawStreamHandler::bufType type, uint8_t *data, long len) {
    bool flushSuccess = false;
    switch (type) {
        case FrameBuf:
            if (len > FRAME_BUFFER_SIZE)
                return 1; // Because the amount of data we receive cannot be flushed into a single frame


            // Check if one framebuf is non flushable
            if (frameFlushable[0] == false) {
                // This means first frameBuf is available

                frameMutex[0].lock();
                // Critical section, trying to flush to buffer
                if (len + frameOffset[0] < FRAME_BUFFER_SIZE) {
                    memcpy(frameBuf[0] + frameOffset[0], data, len);
                    frameOffset[0] += len;
                    flushSuccess = true;
                    if (frameOffset[0] == FRAME_BUFFER_SIZE - 1)
                        frameFlushable[0] = true;
                } else {
                    frameFlushable[0] = true;
                    // Owing to the fact that we were not able to push in this data
                    // and all frame data push are equal sized, cleaning this buffer makes sense
                }

                frameMutex[0].unlock();
            }
            if (flushSuccess == false && frameFlushable[1] == false) {
                // This means first frameBuf is available

                frameMutex[1].lock();
                // Critical section, trying to flush to buffer
                if (len + frameOffset[1] < FRAME_BUFFER_SIZE) {
                    memcpy(frameBuf[1] + frameOffset[1], data, len);
                    frameOffset[1] += len;
                    flushSuccess = true;
                    if (frameOffset[1] == FRAME_BUFFER_SIZE - 1)
                        frameFlushable[1] = true;
                } else {
                    frameFlushable[1] = true;
                    // Owing to the fact that we were not able to push in this data
                    // and all frame data push are equal sized, cleaning this buffer makes sense
                }

                frameBuf[1].unlock();
            }
            if (flushSuccess == false)
                return 1; // Because both the frames are to be flushed and we have no space to put new data
            break;

        case metaBuf:
            if (len > META_BUFFER_SIZE)
                return 1; // Because the amount of data we receive cannot be flushed into a single meta

            // Check if one metabuf is non flushable
            if (metaFlushable[0] == false) {
                // This means first metaBuf is available

                metaMutex[0].lock();
                // Critical section, trying to flush to buffer
                if (len + metaOffset[0] < META_BUFFER_SIZE) {
                    memcpy(metaBuf[0] + metaOffset[0], data, len);
                    metaOffset[0] += len;
                    flushSuccess = true;
                    if (metaOffset[0] == META_BUFFER_SIZE - 1)
                        metaFlushable[0] = true;
                } else {
                    metaFlushable[0] = true;
                    // Owing to the fact that we were not able to push in this data
                    // and all meta data push are equal sized, cleaning this buffer makes sense
                }

                metaMutex[0].unlock();
            }
            if (flushSuccess == false && metaFlushable[1] == false) {
                // This means first frameBuf is available

                metaMutex[1].lock();
                // Critical section, trying to flush to buffer
                if (len + metaOffset[1] < META_BUFFER_SIZE) {
                    memcpy(metaBuf[1] + metaOffset[1], data, len);
                    metaOffset[1] += len;
                    flushSuccess = true;
                    if (metaOffset[1] == META_BUFFER_SIZE - 1)
                        metaFlushable[1] = true;
                } else {
                    metaFlushable[1] = true;
                    // Owing to the fact that we were not able to push in this data
                    // and all meta data push are equal sized, cleaning this buffer makes sense
                }

                metaMutex[1].unlock();
            }
            if (flushSuccess == false)
                return 1; // Because both the metas are to be flushed and we have no space to put new data

            break;
    }
    if (flushSuccess == true)
        return 0;
}


#define