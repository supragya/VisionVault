#include "metaman.h"
#include "../common/mlv.h"
#include <fstream>
#include <iostream>
#include <memory.h>

using namespace RawStreamHandler

void RawStreamHandler::MetaManEntry(const char *metaStreamLoc, const char *metaCache) {
    // Create Buffer
    int buffersize = 128 * 1024 * 1024;
    char *buf = new char[buffersize];

    std::ifstream metaStream(metaStreamLoc, std::ios::binary | std::ios::in);

    if (!metaStream.is_open()) {
        std::cout << "MetaMan: Could not access meta stream " << metaStreamLoc << std::endl;
        return;
    } else {
        std::cout << "MetaMan: Success in accessing stream " << metaStreamLoc << std::endl;
    }

    std::map<std::string, uint8_t> sizeChart = getMlvSizes();
    int offset = 0;
    uint8_t *blockType[4];
    std::map<std::string, uint8_t>::iterator iter;
    while(!metaStream.eof()){
        metaStream.read(reinterpret_cast<char *>(blockType), 4);
        iter = sizeChart.find(std::string(reinterpret_cast<char *>(blockType)));
        if(iter == sizeChart.end()){
            std::cout<<"MetaMan: Could not find size of meta block in sizeChart"<<std::endl;
            return;
        }
        memcpy(buf+offset, blockType, 4);
        offset += 4;
        metaStream.read(buf + offset, iter->second - 4);
        offset += iter->second - 4;

        if(offset > buffersize){
            std::cout<<"MetaMan: Buffer overflow error! "<<std::endl;
            return;
        }
    }
    std::cout<<"MetaMan: metaStream read into memory buffer"<<std::endl;

    std::ofstream out(metaCache, std::ios::binary|std::ios::out);
    if(!out.is_open()){
        std::cout<<"MetaMan: Cache file could not be opened "<<metaCache<<std::endl;
    }
    else{
        out.write(buf, offset);
        std::cout<<"MetaMan: Cache file writing done"<<metaCache<<std::endl;
    }
    out.close();
    metaStream.close();

    std::cout<<"MetaMan: MetaStream ended"<<std::endl;
}

std::map<std::string, uint8_t> getMlvSizes() {
    std::map<std::string, uint8_t> sizeChart;
    sizeChart.insert(std::pair<std::string, uint8_t>("MLVI", 52));
    sizeChart.insert(std::pair<std::string, uint8_t>("RAWI", 20+ sizeof(raw_info_t)));
    sizeChart.insert(std::pair<std::string, uint8_t>("EXPO", 40));
    sizeChart.insert(std::pair<std::string, uint8_t>("LENS", 96));
    sizeChart.insert(std::pair<std::string, uint8_t>("RTCI", 44));
    return sizeChart;
};