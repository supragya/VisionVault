//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include "buffers.h"
#include "controlman.h"
#include "diskman.h"
#include "frameman.h"
#include "metaman.h"

using namespace std;

int main() {
    cout << "--------------------------------------------------------------------------" << endl;
    cout << "AXIOM RVCF Emulation - in memory benchmarking" << endl;
    cout << "Begin allocating space for globalBuffer" << endl;

    RawStreamHandler::rBuf buffer;
    buffer.bufInit();

    cout << "Spawning threads" << endl;

    thread frameThread(RawStreamHandler::FrameManThreadEntry, &buffer);
    thread metaThread(RawStreamHandler::MetaManThreadEntry, &buffer);
    thread diskThread(RawStreamHandler::DiskManagerThreadEntry, &buffer);

    frameThread.join();
    metaThread.join();
    diskThread.join();

    buffer.bufEnd();

    cout << "Emulation ends here" << endl;
    cout << "--------------------------------------------------------------------------" << endl;

    return 0;
}