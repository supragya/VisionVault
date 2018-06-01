//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include <fstream>
#include "frameman.h"
#include "metaman.h"

using namespace std;

void joinCache(const char *finalMlv, const char cache[], int size) {
    ifstream tempInfile;
    ofstream out(finalMlv);
    if (!out.is_open()) {
        std::cout << "Join: Error opening finalMLV file: " << finalMlv << endl;
        return;
    } else {
        std::cout << "Join: success opening " << finalMlv << std::endl;
    }

    int buffersize = 32*1024*1024;
    cout<<"Join: using buffer sized "<<buffersize<<" bytes ("<<buffersize/(1024*1024)<<" MiB)"<<endl;

    for (int i = 0; i < size; i++) {
        cout<<"Join: Begin cache joining: ("<<i<<") "<<cache[i]<<endl;

        tempInfile.open(reinterpret_cast<const char*>(cache[i]), ios::in);
        if (!out.is_open()) {
            std::cout << "Join: Error opening file: " << cache[i] << endl;
            return;
        } else {
            std::cout << "Join: success opening " << cache[i] << std::endl;
        }

        tempInfile.seekg(ios::end);
        long long filesize = tempInfile.tellg();
        std::cout<<cache[i]<<" is sized "<<filesize<<" bytes"<<endl;
    }
}

int main() {
    cout << ">>> AXIOM RVCF Emulation - disk emulation\n" << endl;
    cout << "Spawning threads - frame and meta" << endl;

    const char frameStreamLoc[] = "/tmp/rvcfEmulation/frameStream.dat";
    const char metaStreamLoc[] = "/tmp/rvcfEmulation/metaStream.dat";
    const char frameCache[] = "frameCache.dat";
    const char metaCache[] = "metaCache.dat";
    const char finalMlv[] = "axiomCache.mlv";

    thread frameThread(RawStreamHandler::FrameManEntry, frameStreamLoc, frameCache);
    //thread metaThread(RawStreamHandler::MetaManEntry, metaStreamLoc, metaCache);
    frameThread.join();
    //metaThread.join();

    cout << "Done receiving from streams, begin merge to single cache" << endl;

    joinCache(finalMlv, {metaCache, frameCache}, 2);

    cout << "Cache joining done, mlv is now ready at " << finalMlv << endl;

    cout << "\n>>>Emulation ends here" << endl;


    return 0;
}