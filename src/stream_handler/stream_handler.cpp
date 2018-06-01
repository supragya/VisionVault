//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include "frameman.h"
#include "metaman.h"

using namespace std;

void joinCache(const char *finalMlv, const char *metaCache, const char *frameCache) {

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

    joinCache(finalMlv, metaCache, frameCache);

    cout << "Cache joining done, mlv is now ready at " << finalMlv << endl;

    cout << "\n>>>Emulation ends here" << endl;


    return 0;
}