#ifndef RVCF_FRAME_H
#define RVCF_FRAME_H

#include <mutex>

namespace RawStreamHandler {
    struct FrameBuffer {
        char *buf[2];
        uint bufsize;
        bool filled[2];
        uint offset[2];
        std::mutex mutex[2];
    };

    void FrameManEntry();

    void FrameStreamHandler(bool *syncbool, FrameBuffer *buf, const char *streamloc);

    void FrameDiskHandler(bool *syncbool, FrameBuffer *buf, const char *dumploc);
}

#endif