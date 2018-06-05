//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include <fstream>
#include "frameman.h"
#include "metaman.h"
#include "../common/mlv.h"

#define min(a, b) (a>b ? b : a)


using namespace std;

void joinCache(const char *finalMlv, const char *cache[], int size) {
    ifstream tempInfile;
    ofstream out(finalMlv, ios::binary | ios::out);

    if (!out.is_open()) {
        std::cout << "Join: Error opening finalMLV file: " << finalMlv << endl;
        return;
    } else {
        std::cout << "Join: success opening " << finalMlv << std::endl;
    }

    tempInfile.open(reinterpret_cast<const char *>(cache[0]), ios::binary | ios::in);
    if (!tempInfile.is_open()) {
        std::cout << "Join: Error opening file: " << cache[0] << endl;
        return;
    } else {
        std::cout << "Join: success opening " << cache[0] << std::endl;
    }
    cout << "Out from [" << out.tellp();
    out << tempInfile.rdbuf();
    cout << ", " << out.tellp() << "]" << endl;
    std::cout << "Join: Written successfully " << cache[0] << std::endl;
    tempInfile.close();

    tempInfile.open(reinterpret_cast<const char *>(cache[1]), ios::binary);
    if (!tempInfile.is_open()) {
        std::cout << "Join: Error opening file: " << cache[1] << endl;
        return;
    } else {
        std::cout << "Join: success opening " << cache[1] << std::endl;
    }
    int marker = 0;
    bool markerFound = false;
    tempInfile.read(reinterpret_cast<char *>(&marker), sizeof(int));
    if (marker == 233)
        markerFound = true;
    mlv_vidf_hdr_t vidf_hdr;
    char *buf = new char[18 * 1024 * 1024];
    int framectr = 0;
    while (markerFound) {
        tempInfile.read(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        tempInfile.read(buf, 18 * 1024 * 1024);
        out.write(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        out.write(buf, 18 * 1024 * 1024);
        cout << "\rEncountered frame no. "<<framectr;
        framectr++;
        markerFound = false;
        marker = 0;
        tempInfile.read(reinterpret_cast<char *>(&marker), sizeof(int));
        if (marker == 233)
            markerFound = true;
    }
    cout << endl;
    cout << "Join: written " << framectr << " frames" << endl;
    std::cout << "Join: Written successfully " << cache[1] << std::endl;

    std::cout << "Joining done!" << std::endl;
    out.close();
}

int main() {
    cout.setf(std::ios_base::unitbuf);
    cout << ">>> AXIOM RVCF Emulation - disk emulation\n" << endl;
    cout << "Spawning threads - frame and meta" << endl;

    const char frameStreamLoc[] = "frameData.dat";
    const char metaStreamLoc[] = "metaData.dat";
    const char frameCache[] = "frameCache.dat";
    const char metaCache[] = "metaCache.dat";
    const char finalMlv[] = "axiom.mlv";

    auto start = std::chrono::system_clock::now();
    thread frameThread(RawStreamHandler::FrameManEntry, frameStreamLoc, frameCache);
    thread metaThread(RawStreamHandler::MetaManEntry, metaStreamLoc, metaCache);
    frameThread.join();
    metaThread.join();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_second_ = end - start;
    double elapsed_second = elapsed_second_.count();
    cout << "Done receiving from streams, begin merge to single cache" << endl;
    std::cout << "Caching took: (seconds) " << elapsed_second << std::endl;
    const char *caches[2] = {metaCache, frameCache};


    joinCache(finalMlv, caches, 2);

    auto end2 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_second__ = end2 - end;
    elapsed_second = elapsed_second__.count();

    std::cout << "Joining took: (seconds) " << elapsed_second << std::endl;
    cout << "Cache joining done, mlv is now ready at " << finalMlv << endl;

    std::chrono::duration<double> elapsed_second___ = end2 - start;
    elapsed_second = elapsed_second___.count();
    std::cout << "Net time took: (seconds) " << elapsed_second << std::endl;
    cout << "\n>>>Emulation ends here" << endl;


    return 0;
}