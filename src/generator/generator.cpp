//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

// TODO: Does using MLV code make us conform to GNU GPLv2 too?

#include <iostream>
#include <memory.h>
#include <fstream>
#include "generator.h"

using namespace std;

int main() {
    const char *frameFile = "frameData.dat";
    const char *metaFile = "metaData.dat";
    const int framesCount = 100;

    cout.setf(std::ios_base::unitbuf);
    cout << "Axiom Stream Generator" << endl;

    ofstream frames(frameFile, ios::binary | ios::trunc | ios::out);
    ofstream meta(metaFile, ios::binary | ios::trunc | ios::out);

    if (!frames.is_open() || !meta.is_open()) {
        cout << "Generator could not open either of frames or meta file" << endl;
        return 1;
    }

    // Frame file generation

    char *raw12Data = Zeros(18 * 1024 * 1024);
    mlv_vidf_hdr_t vidf_hdr;
    Populate(&vidf_hdr);

    for (int i = 0; i < framesCount; i++) {
        frames.write(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        frames.write(raw12Data, 18 * 1024 * 1024);
    }

    // Meta file generation

    mlv_file_hdr_t file_hdr;
    Populate(&file_hdr);
    mlv_rawi_hdr_t rawi_hdr;
    Populate(&rawi_hdr);

    char *raw_info = new char[sizeof(raw_info_t)];
    // TODO: populate raw_info_t

    mlv_expo_hdr_t expo_hdr;
    Populate(&expo_hdr);
    mlv_lens_hdr_t lens_hdr;
    Populate(&lens_hdr);

    meta.write(reinterpret_cast<char *>(&file_hdr), sizeof(mlv_file_hdr_t));
    meta.write(reinterpret_cast<char *>(&rawi_hdr), sizeof(mlv_rawi_hdr_t));
    meta.write(raw_info, sizeof(raw_info_t));
    meta.write(reinterpret_cast<char *>(&expo_hdr), sizeof(mlv_expo_hdr_t));
    meta.write(reinterpret_cast<char *>(&lens_hdr), sizeof(mlv_lens_hdr_t));

    frames.close();
    meta.close();

    cout << "Generator ends, stream files written."<<endl;

    return 0;
}

void Populate(mlv_file_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->fileMagic)), "MLVI", 4);
    block->blockSize = 52;
    block->fileGuid = 0; // TODO: Add fileGuid
    block->fileNum = 0;
    block->fileCount = 1;
    block->fileFlags = 0;
    block->videoClass = 1;
    block->audioClass = 0;
    block->videoFrameCount = 0; // Autodetect
    block->audioFrameCount = 0;
    block->sourceFpsNom = 30;
    block->sourceFpsDenom = 1; // Using 30fps video
}

void Populate(mlv_rawi_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "RAWI", 4);
    block->blockSize = 20 + sizeof(raw_info_t);
    block->timestamp = 0;
    block->xRes = 4096;
    block->yRes = 3072;
    // Zeros(reinterpret_cast<uint8_t *>(&(block->raw_info)), sizeof(raw_info_t));
}

void Populate(mlv_vidf_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "VIDF", 4);
    block->blockSize = 32; // Check if it is correct (header size or header + frame size)
    block->timestamp = 0;
    block->frameNumber = 0;
    block->cropPosX = 0;
    block->cropPosY = 0;
    block->panPosX = 4096;
    block->panPosY = 3072;
}

void Populate(mlv_expo_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "EXPO", 4);
    block->blockSize = 40;
    block->timestamp = 0;
    block->isoMode = 0;
    block->isoValue = 400;
    block->isoAnalog = 800;
    block->digitalGain = 1;
    block->shutterValue = 250;
}

void Populate(mlv_lens_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "LENS", 4);
    block->blockSize = 96;
    block->timestamp = 0;
    block->focalLength = 35;
    block->focalDist = 65535;
    block->aperture = 350;
    block->stabilizerMode = 0;
    block->autofocusMode = 1;
    block->flags = 0;
    block->lensID = 0x4FFAE214;
    strcpy(reinterpret_cast<char *>(&(block->lensName)), "Tamron Di LD Macro 1:2");
    strcpy(reinterpret_cast<char *>(&(block->lensSerial)), "TMR_AXFF60213F2");
}

char *Zeros(int size) {
    char *ret = new char[size];
    for (int i = 0; i < size; i++)
        ret[i] = 0;
    return ret;
}