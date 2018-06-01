//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

// TODO: Does using MLV code make us conform to GNU GPLv2 too?

#include <iostream>
#include <memory.h>
#include "generator.h"

using namespace std;

int main(){
    cout.setf(std::ios_base::unitbuf);
    cout<<"Axiom Stream Generator"<<endl;



    return 0;
}

void Populate(mlv_file_hdr_t* block){
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

void Populate(mlv_rawi_hdr_t* block){
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "RAWI", 4);
    block->blockSize = 20 + sizeof(raw_info_t);
    block->timestamp = 0;
    block->xRes = 4096;
    block->yRes = 3072;
    Zeros(reinterpret_cast<uint8_t*>(&(block->raw_info)), sizeof(raw_info_t));
}

void Populate(mlv_vidf_hdr_t* block){
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "VIDF", 4);
    block->blockSize = 32; // Check if it is correct (header size or header + frame size)
    block->timestamp = 0;
    block->frameNumber = 0;
    block->cropPosX = 0;
    block->cropPosY = 0;
    block->panPosX = 4096;
    block->panPosY = 3072;
}

void Populate(mlv_expo_hdr_t* block){
    memcpy(reinterpret_cast<char *>(&(block->blockType)), "EXPO", 4);
    block->blockSize = 40;
    block->timestamp = 0;
    block->isoMode = 0;
    block->isoValue = 400;
    block->isoAnalog = 800;
    block->digitalGain = 1;
    block->shutterValue = 250;
}

void Populate(mlv_lens_hdr_t* block){
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

char* Zeros(int size){
    char *ret = new char[size];
    for(int i=0; i<size; i++)
        ret[i] = 0;
}