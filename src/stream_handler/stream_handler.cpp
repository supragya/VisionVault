//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include <fstream>
#include "frameman.h"
#include "metaman.h"

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

    int buffersize = 32 * 1024 * 1024;
    char *buf = new char[buffersize];
    cout << "Join: using buffer sized " << buffersize << " bytes (" << buffersize / (1024 * 1024) << " MiB)" << endl;

    for (int i = 0; i < size; i++) {
        cout << "Join: Begin cache joining: (" << i << ") " << cache[i] << endl;

        tempInfile.open(reinterpret_cast<const char *>(cache[i]), ios::binary | ios::in);
        if (!out.is_open()) {
            std::cout << "Join: Error opening file: " << cache[i] << endl;
            return;
        } else {
            std::cout << "Join: success opening " << cache[i] << std::endl;
        }

        tempInfile.seekg(tempInfile.end);
        long filesize = tempInfile.tellg();
        tempInfile.seekg(tempInfile.beg);
        std::cout << cache[i] << " is sized " << filesize << " bytes" << endl;

        while(min(filesize, buffersize) == buffersize){
            tempInfile.read(buf, buffersize);
            out.write(buf, buffersize);
        }
        tempInfile.read(buf, filesize);
        out.write(buf, filesize);

        std::cout<< "Join: Written successfully "<<cache[i]<<std::endl;
    }

    delete buf;
    std::cout<<"Joining done!"<<std::endl;
    out.close();
}

int main() {
    cout << ">>> AXIOM RVCF Emulation - disk emulation\n" << endl;
    cout << "Spawning threads - frame and meta" << endl;

    const char frameStreamLoc[] = "frameData.dat";
    const char metaStreamLoc[] = "metaData.dat";
    const char frameCache[] = "frameCache.dat";
    const char metaCache[] = "metaCache.dat";
    const char finalMlv[] = "axiom.mlv";

    thread frameThread(RawStreamHandler::FrameManEntry, frameStreamLoc, frameCache);
    thread metaThread(RawStreamHandler::MetaManEntry, metaStreamLoc, metaCache);
    frameThread.join();
    metaThread.join();

    cout << "Done receiving from streams, begin merge to single cache" << endl;

    const char *caches[2] = {metaCache, frameCache};

    joinCache(finalMlv, caches, 2);

    cout << "Cache joining done, mlv is now ready at " << finalMlv << endl;

    cout << "\n>>>Emulation ends here" << endl;


    return 0;
}