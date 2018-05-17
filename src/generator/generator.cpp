//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

// TODO: Does using MLV code make us conform to GNU GPLv2 too?

#include <iostream>
#include <fstream>
#include "generator.h"
#include "../common/mlv.h"
using namespace std;

// Synthetic RAW12 generator
// type = 1 (red), 2 (green), 3 (blue)
uint8_t* GetRaw12Frame(int type){
    uint8_t* rawFrame = new uint8_t[18*1024*1024];

    // Using RGGB pattern
    int curptr = 0;
    for(int i = 0; i<3072/2; i++){
        // All row pairs of sensel data
        for(int j=0; j<2; j++){
            if(j == 0){
                // Odd row
                for(int k=0; k<4096/2; k++){
                    rawFrame[curptr++] = type==1 ? 255 : 0;
                    rawFrame[curptr++] = type==1 ? 240 : type == 2? 15 : 0;
                    rawFrame[curptr++] = type==2 ? 255 : 0;
                }
            }
            else{
                // Even row
                for(int k=0; k<4096/2; k++){
                    rawFrame[curptr++] = type==2 ? 255 : 0;
                    rawFrame[curptr++] = type==2 ? 240 : type == 3? 15 : 0;
                    rawFrame[curptr++] = type==3 ? 255 : 0;
                }
            }
        }
    }

    return rawFrame;
}

int main(){
    fileStream frameStream("FrameStream.dat", streamType.frame);

    // Begin writing to FrameData
    cout<<"Begin writing FrameStream "<<endl;

    // MLVI block to begin the frame stream
    mlv_file_hdr_t mlviHdr;
    Populate(&mlviHdr);
    frameStream.write2file(reinterpret_cast<void*>(mlviHdr), sizeof(mlviHdr));

    // RAWI block to set raw mode
    mlv_rawi_hdr_t rawiHdr;
    Populate(&rawiHdr);
    frameStream.write2file(reinterpret_cast<void*>(rawiHdr), sizeof(rawiHdr));
    cout<<"Headers written, begin writing frames"<<endl;

    // Get a few Red/Green/Blue RAW12 frames (synthetic)
    uint8_t *colorFrames[] = {GetRaw12Frame(1), GetRaw12Frame(2), GetRaw12Frame(3)};
    mlv_vidf_hdr_t templateVidf;
    Populate(&templateVidf);

    // Write Frames 300 in number
    const int NUM_3FRAMES = 100;
    for(int i=0; i<NUM_3FRAMES;){
        for(int j=0; j<3; j++){
            templateVidf.frameNumber = i*3+j;
            templateVidf.timestamp = i*3+j;
            frameStream.write2file(templateVidf, sizeof(templateVidf));
            frameStream.write2file(colorFrames[j], 18*1024*1024);
        }
        i++;
        cout<<"\rProgress:[";
        for(int j=0; j<i; j = j+NUM_3FRAMES/10)
            cout<<'=';
        cout<<'>';
        for(int j=i+1; j<NUM_3FRAMES; j+=NUM_3FRAMES/10)
            cout<<'.';
        cout<<i*100/NUM_3FRAMES<<'%';
    }
    cout<<" Done."<<endl;

    // Begin writing metadata

    cout<<"Begin writing MetaStream "<<endl;
    fileStream metaStream("MetaStream.dat", streamType.meta);

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
    block->raw_info = Zeros(sizeof(raw_info_t));
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