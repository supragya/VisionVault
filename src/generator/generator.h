//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#ifndef GENERATOR_H
#define GENERATOR_H

#include <fstream>
#include <string>
#include "../common/mlv.h"

using namespace std;

enum streamType_{
    frame,
    meta
} streamType;

class fileStream{
private:
    std::ofstream file;
    streamType_ type;
public:
    fileStream(string fileName, bool sType){
        file.open(fileName, ios::out| ios::trunc);
        type = sType ? frame : meta;

        cout<<"FileOpen: "<<fileName<<" "<< (file.is_open() ? "Success ": "Failure ");
        cout<<", StreamType: "<< (sType ? "Frame " : "Meta " )<<endl;
    }
    bool write2file(const char* data, int len){
        if(file.is_open()){
            file.write(data, len);
        }
    }
    ~fileStream(){
        // Cleanup
        file.close();
    }
};

void Populate(mlv_vidf_hdr_t*);
void Populate(mlv_rawi_hdr_t*);
void Populate(mlv_file_hdr_t*);
void Populate(mlv_lens_hdr_t*);
void Populate(mlv_expo_hdr_t*);

void Zeros(uint8_t* loc, uint32_t size);
#endif