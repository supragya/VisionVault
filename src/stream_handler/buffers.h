//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//


#ifndef RVCFBUFFERS_H
#define RVCFBUFFERS_H

#include <mutex>

#define FRAME_BUFFER_SIZE 128*1024*1024 // 128 MB
#define META_BUFFER_SIZE 32*1024*1024 // 32 MB

namespace RawStreamHandler {
    enum bufType {
        FrameBuf,
        MetaBuf
    };

    class rBuf {
    public:
        std::mutex frameMutex[2];
        std::mutex metaMutex[2];

        long frameOffset[2];
        long metaOffset[2];

        bool frameFlushable[2];
        bool metaFlushable[2];

        char *frameBuf[];
        char *metaBuf[];

        int bufInit() {
            frameBuf[0] = new char[FRAME_BUFFER_SIZE];
            frameBuf[1] = new char[FRAME_BUFFER_SIZE];

            metaBuf[0] = new char[META_BUFFER_SIZE];
            metaBuf[1] = new char[FRAME_BUFFER_SIZE];

            frameOffset[0] = frameOffset[1] = 0;
            metaOffset[0] = metaOffset[1] = 0;

            frameFlushable[0] = frameFlushable[1] = false;
            metaFlushable[0] = metaFlushable[1] = false;
        }

        int bufEnd() {
            delete frameBuf[0];
            delete frameBuf[1];
            delete metaBuf[0];
            delete metaBuf[1];
        }

        int pushData(bufType type, uint8_t *data, long len);

    };
}

#endif //RVCF_BUFFERS_H
