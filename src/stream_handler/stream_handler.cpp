//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include "frameman.h"
#include "metaman.h"

using namespace std;

int main() {
    cout << "AXIOM RVCF Emulation - disk benchmarking" << endl;
    cout << "Spawning threads - frame and meta" << endl;

    const char frameStreamLoc[] = "/tmp/rvcfEmulation/frameStream.dat";
    const char frameCache[] = "frameCache.dat";
    const char metaCache[] = "metaCache.dat";
    const char finalMlv[] = "axiomCache.mlv";

    thread frameThread(RawStreamHandler::FrameManEntry, frameStreamLoc, frameCache);

    frameThread.join();


    cout << "Emulation ends here" << endl;


    return 0;
}