#include "../common/mlv.h"
#include "frameman.h"
#include <thread>

using namespace RawStreamHandler;

void RawStreamHandler::FrameManEntry() {
    // Create new struct
    FrameBuffer fb;
    fb.bufsize = 128 * 1024 * 1024; // 128MB
    fb.buf[0] = new char[fb.bufsize];
    fb.buf[1] = new char[fb.bufsize];
    fb.filled[0] = fb.filled[1] = false;
    fb.offset[0] = fb.offset[1] = 0;

    bool syncbool = true;
    std::thread StreamHandler(FrameStreamHandler, &syncbool, &fb, "/tmp/.../");
    std::thread DiskHandler(FrameDiskHandler, &syncbool, &fb, "frameDump.dat");

    StreamHandler.join();
    DiskHandler.join();
}

void RawStreamHandler::FrameStreamHandler(bool *syncbool, FrameBuffer *buf, const char *streamloc) {
    return;
}

void RawStreamHandler::FrameDiskHandler(bool *syncbool, FrameBuffer *buf, const char *dumploc){
    return;
}