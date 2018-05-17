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

#endif