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

uint32_t VIDF_FRAMESIZE = 12*4096*3072/8;

using namespace std;

int main() {
    const char *frameFile = "frameData.dat";
    const char *metaFile = "metaData.dat";
    const char *test12 = "../test_raw12/test.raw12";
    const int framesCount = 1;

    cout.setf(std::ios_base::unitbuf);
    cout << "Axiom Stream Generator" << endl;

    ofstream frames(frameFile, ios::binary | ios::trunc | ios::out);
    ofstream meta(metaFile, ios::binary | ios::trunc | ios::out);

    if (!frames.is_open() || !meta.is_open()) {
        cout << "Generator could not open either of frames or meta file" << endl;
        return 1;
    }

    // Frame file generation

    char *raw12Data = Zeros(12 * 4096 * 3072 / 8);
    ifstream rawFile(test12, ios::binary | ios::in);
    if(!rawFile.is_open())
        cout<<"Error reading RAW12 file"<<endl;
    else
        rawFile.read(raw12Data, 12 * 4096 * 3072 / 8);
    rawFile.close();
    mlv_vidf_hdr_t vidf_hdr = {0};
    Populatevidf(&vidf_hdr);

    int marker = 233;
    cout << "Generating Frames\n";
    for (int i = 0; i < framesCount; i++) {
        vidf_hdr.frameNumber = i;
        vidf_hdr.timestamp = (i+3)*100000;
        frames.write(reinterpret_cast<char *>(&marker), sizeof(int));
        frames.write(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        cout<<"["<<VIDF_FRAMESIZE<<"]";
        frames.write(raw12Data, VIDF_FRAMESIZE);
        cout << ".. Of " << framesCount << " written " << i + 1<<endl;
    }
    cout << endl;

    // Meta file generation

    mlv_file_hdr_t file_hdr = {0};
    Populatefile(&file_hdr);
    mlv_rawi_hdr_t rawi_hdr = {0};
    Populaterawi(&rawi_hdr);

    mlv_expo_hdr_t expo_hdr = {0};
    Populateexpo(&expo_hdr);
    mlv_lens_hdr_t lens_hdr = {0};
    Populatelens(&lens_hdr);
    mlv_rtci_hdr_t rtci_hdr = {0};
    Populatertci(&rtci_hdr);

    cout << "Generating Meta ";
    meta<<233;
    meta.write(reinterpret_cast<char *>(&file_hdr), sizeof(mlv_file_hdr_t));
    meta<<233;
    meta.write(reinterpret_cast<char *>(&rawi_hdr), sizeof(mlv_rawi_hdr_t));
    //meta.write(reinterpret_cast<char *>(&rawi_hdr.raw_info), sizeof(raw_info_t));
    meta<<233;
    meta.write(reinterpret_cast<char *>(&expo_hdr), sizeof(mlv_expo_hdr_t));
    meta<<233;
    meta.write(reinterpret_cast<char *>(&lens_hdr), sizeof(mlv_lens_hdr_t));
    meta<<233;
    meta.write(reinterpret_cast<char *>(&rtci_hdr), sizeof(mlv_rtci_hdr_t));
    cout << "done" << endl;

    frames.close();
    meta.close();

    cout << "Generator ends, stream files written." << endl;

    return 0;
}

void Populatefile(mlv_file_hdr_t *block) {
    memcpy(reinterpret_cast<char *>(&(block->fileMagic)), "MLVI", 4);
    memcpy(reinterpret_cast<char *>(&(block->versionString)), "v2.0", 5);
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
    block->blockSize = sizeof(mlv_vidf_hdr_t) + VIDF_FRAMESIZE; // Check if it is correct (header size or header + frame size)
    block->timestamp = 0;
    block->frameNumber = 0;
    block->cropPosX = 0;
    block->cropPosY = 0;
    block->panPosX = 4096;
    block->panPosY = 3072;
    block->frameSpace = 0;
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

void Populatertci(mlv_rtci_hdr_t *block){
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "RTCI", 4);
    block->blockSize = sizeof(mlv_rtci_hdr_t);
    block->timestamp = 1;
    block->tm_sec = 4;
    block->tm_min = 55;
    block->tm_hour = 4;
    block->tm_mday = 8;
    block->tm_mon = 6;
    block->tm_year = 118;
    block->tm_wday = 0;
    block->tm_yday = 188;
    block->tm_isdst = 0;
    block->tm_gmtoff = 0;
    memcpy(reinterpret_cast<char *>(&(block->tm_zone)), "GRNWICH", 8);
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
    block->pitch = (block->width*12)/8;
    block->frame_size = block->height*block->pitch;
    block->black_level = 0;
    block->white_level = 15000;
    block->crop.origin[0] = block->crop.origin[1] = 0;
    block->crop.size[0] = 4096;
    block->crop.size[1] = 3072;
    block->exposure_bias[0] = block->exposure_bias[1] = 0;
    block->cfa_pattern = 0x02010100;
    block->calibration_illuminant1 = 0;
    block->dng_active_area[0] = 0;
    block->dng_active_area[1] = 0;
    block->dng_active_area[2] = 3072;
    block->dng_active_area[3] = 4096;
    block->dynamic_range = 12;
    for(int i=0; i<18; i++)
        block->color_matrix1[i] = i*100;
}