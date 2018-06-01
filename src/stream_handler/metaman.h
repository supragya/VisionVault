#ifndef RVCF_META_H
#define RVCF_META_H

#include <map>
#include <string>

namespace RawStreamHandler{
    std::map<std::string, uint8_t> getMlvSizes();
    void MetaManEntry(const char* metaStreamLoc, const char* metaCache);
}

#endif