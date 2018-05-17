//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#ifndef GENERATOR_H
#define GENERATOR_H

#include <fstream>
#include <string>
using namespace std;

enum streamType{
    frame,
    meta
};

class fileStream{
private:
    std::ofstream file;
    streamType type;
public:
    fileStream(string fileName, bool sType){
        file.open(fileName, ios::out| ios::trunc);
        type = sType ? streamType.frame : streamType.meta;

        cout<<"FileOpen: "<<fileName<<" "<< file.is_open() ? "Success ": "Failure ";
        cout<<", StreamType: "<< sType ? "Frame " : "Meta "<<endl;
    }
    bool write2file(void* data, int len){
        if(file.is_open()){
            file.write(data, len);
        }
    }
    ~fileStream(){
        // Cleanup
        file.close();
    }
};

typedef struct {
    uint32_t api_version;           // increase this when changing the structure
    uint32_t do_not_use_this;       // was the memory buffer, this can't work on 64-bit systems

    int32_t height, width, pitch;
    int32_t frame_size;
    int32_t bits_per_pixel;         // 14

    int32_t black_level;            // autodetected
    int32_t white_level;            // somewhere around 13000 - 16000, varies with camera, settings etc
    // would be best to autodetect it, but we can't do this reliably yet
    union                           // DNG JPEG info
    {
        struct
        {
            int32_t x, y;           // DNG JPEG top left corner
            int32_t width, height;  // DNG JPEG size
        } jpeg;
        struct
        {
            int32_t origin[2];
            int32_t size[2];
        } crop;
    };
    union                       // DNG active sensor area (Y1, X1, Y2, X2)
    {
        struct
        {
            int32_t y1, x1, y2, x2;
        } active_area;
        int32_t dng_active_area[4];
    };
    int32_t exposure_bias[2];       // DNG Exposure Bias (idk what's that)
    int32_t cfa_pattern;            // stick to 0x02010100 (RGBG) if you can
    int32_t calibration_illuminant1;
    int32_t color_matrix1[18];      // DNG Color Matrix
    int32_t dynamic_range;          // EV x100, from analyzing black level and noise (very close to DxO)
} raw_info_t;


#endif