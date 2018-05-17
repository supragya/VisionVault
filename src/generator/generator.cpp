//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

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

    // MLVI block to begin the frame stream
    mlv_file_hdr_t mlviHdr;
    Populate(&mlviHdr);
    frameStream.write2file(reinterpret_cast<void*>(mlviHdr), sizeof(mlviHdr));

    // RAWI block to set raw mode
    mlv_rawi_hdr_t rawiHdr;
    Populate(&rawiHdr);
    frameStream.write2file(reinterpret_cast<void*>(rawiHdr), sizeof(rawiHdr));

    // Get a few Red/Green/Blue RAW12 frames (synthetic)
    uint8_t *colorFrames[] = {GetRaw12Frame(1), GetRaw12Frame(2), GetRaw12Frame(3)};
    mlv_vidf_hdr_t templateVidf;
    Populate(&templateVidf);

    // Write Frames 300 in number
    for(int i=0; i<100; i++){
        for(int j=0; j<3; j++){
            templateVidf.timestamp = i*3+j;
            frameStream.write2file(templateVidf, sizeof(templateVidf));
            frameStream.write2file(colorFrames[j], 18*1024*1024);
        }
    }

    // Begin writing metadata
    fileStream metaStream("MetaStream.dat", streamType.meta);


    return 0;
}