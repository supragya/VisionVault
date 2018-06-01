#include "../common/mlv.h"
#include "frameman.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <memory.h>

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
    int exit_reason = 0;
    // Reasons: noFileStream(0), syncbool(1), endofstream(2)

    std::ifstream fStream(streamloc);

    if (!fStream.is_open()) {
        *syncbool = false;
    } else {
        std::cout << "FrameStreamHandler success opening " << streamloc << std::endl;
    }

    if (*syncbool)
        exit_reason = 1;

    mlv_vidf_hdr_t chunk = {0};
    bool entrypossible;
    int i;
    while (*syncbool) {
        if (fStream.eof()) {
            exit_reason = 2;
            break;
        }
        fStream.read(reinterpret_cast<char *>(&chunk), sizeof(chunk));
        entrypossible = false;
        do {
            for (i = 0; i < 2; i++) {
                if (buf->filled[i])
                    continue;
                else if (buf->offset[i] + sizeof(chunk) > buf->bufsize) {
                    buf->filled[i] = true;
                    continue;
                } else {
                    entrypossible = true;
                    break;
                }
            }
        } while (!entrypossible);
        // Entry possible now

        buf->mutex[i].lock();
        memcpy(buf->buf[i] + buf->offset[i], &chunk, sizeof(chunk));
        buf->offset[i] += sizeof(chunk);
        buf->mutex[i].unlock();
    }


    // Report
    std::string ex = "FrameStreamHandler exits: ";
    switch (exit_reason) {
        case 0:
            ex = ex + "Could not access stream " + streamloc;
            break;
        case 1:
            ex = ex + "Syncbool set to false";
            break;
        case 2:
            ex = ex + "Reached eof on file " + streamloc;
            break;
        default:
            ex = ex + "Unknown";
            break;
    }
    std::cout << ex << std::endl;
    *syncbool = false;
}

void RawStreamHandler::FrameDiskHandler(bool *syncbool, FrameBuffer *buf, const char *dumploc) {
    return;
}