//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

// TODO: Does using MLV code make us conform to GNU GPLv2 too?

#include <iostream>
#include <memory.h>
#include <fstream>
#include "generator.h"
#include "../common/mlv.h"

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
    Populatevidf(&vidf_hdr);

    cout << "Generating Frames\n";
    for (int i = 0; i < framesCount; i++) {
        frames.write(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        frames.write(raw12Data, 18 * 1024 * 1024);
        cout << "\rOf " << framesCount << " written " << i + 1;
    }
    cout << endl;

    // Meta file generation

    mlv_file_hdr_t file_hdr;
    Populatefile(&file_hdr);
    mlv_rawi_hdr_t rawi_hdr;
    Populaterawi(&rawi_hdr);

    mlv_expo_hdr_t expo_hdr;
    Populateexpo(&expo_hdr);
    mlv_lens_hdr_t lens_hdr;
    Populatelens(&lens_hdr);

    cout << "Generating Meta ";
    meta.write(reinterpret_cast<char *>(&file_hdr), sizeof(mlv_file_hdr_t));
    meta.write(reinterpret_cast<char *>(&rawi_hdr), sizeof(mlv_rawi_hdr_t));
    //meta.write(reinterpret_cast<char *>(&rawi_hdr.raw_info), sizeof(raw_info_t));
    meta.write(reinterpret_cast<char *>(&expo_hdr), sizeof(mlv_expo_hdr_t));
    meta.write(reinterpret_cast<char *>(&lens_hdr), sizeof(mlv_lens_hdr_t));
    cout << "done" << endl;

    frames.close();
    meta.close();

    cout << "Generator ends, stream files written." << endl;

    return 0;
}

void Populatefile(mlv_file_hdr_t *block) {
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

void Populaterawi(mlv_rawi_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "RAWI", 4);
    block->blockSize = 20 + sizeof(raw_info_t);
    block->timestamp = 0;
    block->xRes = 4096;
    block->yRes = 3072;
    Populaterawinfot(&(block->raw_info));
}

void Populatevidf(mlv_vidf_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "VIDF", 4);
    block->blockSize = 32; // Check if it is correct (header size or header + frame size)
    block->timestamp = 0;
    block->frameNumber = 0;
    block->cropPosX = 0;
    block->cropPosY = 0;
    block->panPosX = 4096;
    block->panPosY = 3072;
}

void Populateexpo(mlv_expo_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "EXPO", 4);
    block->blockSize = 40;
    block->timestamp = 0;
    block->isoMode = 0;
    block->isoValue = 400;
    block->isoAnalog = 800;
    block->digitalGain = 1;
    block->shutterValue = 250;
}

void Populatelens(mlv_lens_hdr_t *block) {
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

void Populaterawinfot(raw_info_t *block){
    block->api_version = 0x0000001;
    block->height = 3072;
    block->width = 4092;
    block->bits_per_pixel = 12;
    block->pitch = block->width*12/8;
    block->frame_size = block->height*block->pitch;
    block->black_level = 0;
    block->white_level = 15000;
    block->crop.origin[0] = block->crop.origin[1] = 0;
    block->crop.size[0] = 4096;
    block->crop.size[1] = 3072;
    block->exposure_bias[0] = block->exposure_bias[1] = 0;
    block->cfa_pattern = 0x02010100;
    block->calibration_illuminant1 = 0;
    block->dynamic_range = 12;
}