#include "StreamHandler.h"

void low_speed_handler(std::string streamloc, std::string output){
    std::ifstream in(streamloc.c_str(), std::ios::binary | std::ios::in);
    std::ofstream out(output.c_str(), std::ios::binary | std::ios::out);

    if(!in.is_open() | !out.is_open()){
        std::cerr<<"[LSH] Cannot open stream location or the output file to write to."<<std::endl;
        return;
    }

    uint8_t blockType[4];
    uint32_t blockSize;

    auto *buffer = new uint8_t[512 * 1024 * 1024];
    uint32_t offset = 0;

    while(!in.eof()){
        in.read(reinterpret_cast<char *>(blockType), 4);
        in.read(reinterpret_cast<char *>(&blockSize), sizeof(uint32_t));

        if(in.fail()){
            std::cout<<"LSH ended stream. Returning"<<std::endl;
            break;
        }

        std::cout<<"LSH "<<(char)blockType[0]<<(char)blockType[1]<<(char)blockType[2]<<(char)blockType[3]<<"\n";

        if(offset + blockSize >= 512 *1024 *1024){
            out.write(reinterpret_cast<char *>(buffer), offset);
            offset = 0;
        }

        memcpy(reinterpret_cast<char *>(buffer) + offset, blockType, 4);
        offset += 4;
        memcpy(reinterpret_cast<char *>(buffer) + offset, &blockSize, sizeof(blockSize));
        offset += sizeof(blockSize);

        in.read(reinterpret_cast<char *>(buffer)+ offset, blockSize - 4 - sizeof(blockSize));
        offset += blockSize - 4 - sizeof(blockSize);
    }

    out.write(reinterpret_cast<char *>(buffer), offset);

    in.close();
    out.close();

    delete buffer;
    return;
}