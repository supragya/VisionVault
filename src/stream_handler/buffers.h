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
        static std::mutex frameMutex[2];
        static std::mutex metaMutex[2];

        static long frameOffset[2];
        static long metaOffset[2];

        static bool frameFlushable[2];
        static bool metaFlushable[2];

        static uint8_t *frameBuf[] = {new uint8_t[FRAME_BUFFER_SIZE], new uint8_t[FRAME_BUFFER_SIZE]};
        static uint8_t *metaBuf[] = {new uint8_t[META_BUFFER_SIZE], new uint8_t[META_BUFFER_SIZE]};

        int aux_push(uint8_t *buf, uint8_t *data, long len);

    public:
        int bufInit();

        int pushData(bufType type, uint8_t *data, long len);

        friend void dm::diskMan();
    };
}

#endif //RVCF_BUFFERS_H
