#include "../common/mlv.h"
#include "frameman.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <memory.h>

using namespace RawStreamHandler;

void RawStreamHandler::FrameManEntry(const char *frameStreamLoc, const char *frameCache) {
    // Create new struct
    FrameBuffer fb;
    fb.bufsize = 128 * 1024 * 1024; // 128MB
    fb.buf[0] = new char[fb.bufsize];
    fb.buf[1] = new char[fb.bufsize];
    fb.filled[0] = fb.filled[1] = false;
    fb.offset[0] = fb.offset[1] = 0;

    bool syncbool = true;
    std::thread StreamHandler(FrameStreamHandler, &syncbool, &fb, frameStreamLoc);
    std::thread DiskHandler(FrameDiskHandler, &syncbool, &fb, frameCache);

    StreamHandler.join();
    DiskHandler.join();
    std::cout << "FrameStreamHandler done" << std::endl;
}

void RawStreamHandler::FrameStreamHandler(bool *syncbool, FrameBuffer *buf, const char *streamloc) {
    int exit_reason = 0;
    // Reasons: noFileStream(0), syncbool(1), endofstream(2)

    std::ifstream fStream(streamloc, std::ios::binary);

    if (!fStream.is_open()) {
        *syncbool = false;
    } else {
        std::cout << "FrameStreamHandler success opening " << streamloc << std::endl;
    }

    if (*syncbool)
        exit_reason = 1;

    mlv_vidf_hdr_t chunk = {0};
    char *vidbuf = new char[18 * 1024 * 1024]; // 18MB
    bool entrypossible;
    int i;
    int numFrames = 0;

    while (*syncbool) {
        if (fStream.eof()) {
            exit_reason = 2;
            break;
        }
        fStream.read(reinterpret_cast<char *>(&chunk), sizeof(chunk));
        fStream.read(reinterpret_cast<char *>(vidbuf), 18 * 1024 * 1024);
        entrypossible = false;
        do {
            for (i = 0; i < 2; i++) {
                if (buf->filled[i])
                    continue;
                else if (buf->offset[i] + sizeof(chunk) + 18 * 1024 * 1024 > buf->bufsize) {
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
        memcpy(buf->buf[i] + buf->offset[i], reinterpret_cast<void *>(vidbuf), 18 * 1024 * 1024);
        buf->offset[i] += 18 * 1024 * 1024;
        buf->mutex[i].unlock();
        numFrames++;
    }

    // Report
    std::cout<<"FrameStreamHandler handled "<<numFrames<<" RAW12 vidf frames."<<std::endl;
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
    fStream.close();
}

void RawStreamHandler::FrameDiskHandler(bool *syncbool, FrameBuffer *buf, const char *dumploc) {
    int exit_reason = 0;
    // Reasons: noFileStream(0), syncbool(1), endofstream(2)

    std::ofstream fStream(dumploc, std::ios::binary);

    if (!fStream.is_open()) {
        exit_reason = 0;
        *syncbool = false;
    } else {
        std::cout << "FrameDiskHandler success opening " << dumploc << std::endl;
    }

    int i;
    while (*syncbool) {
        for (i = 0; i < 2; i++) {
            if (buf->filled[i]) {
                std::cout << "FrameDiskHandler dumping " << i << std::endl;
                buf->mutex[i].lock();
                fStream.write((buf->buf[i]), buf->offset[i]);
                buf->offset[i] = 0;
                buf->filled[i] = 0;
                buf->mutex[i].unlock();
            }
        }
    }
    if (!*syncbool) {
        exit_reason = 1;
        for (i = 0; i < 2; i++) {
            if (buf->filled[i]) {
                std::cout << "FrameDiskHandler dumping " << i << "(after syncbool = false)" << std::endl;
                buf->mutex[i].lock();
                fStream.write((buf->buf[i]), buf->offset[i]);
                buf->offset[i] = 0;
                buf->filled[i] = 0;
                buf->mutex[i].unlock();
            }
        }
    }


    // Report
    std::string ex = "FrameDiskHandler exits: ";
    switch (exit_reason) {
        case 0:
            ex = ex + "Could not access outfile " + dumploc;
            break;
        case 1:
            ex = ex + "syncbool set to false";
            break;
        default:
            ex = ex + "Unknown";
            break;
    }
    std::cout << ex << std::endl;
    *syncbool = false;
    fStream.close();
}