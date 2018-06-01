//
// Created by supragya on 20/5/18.
//

#include <iostream>
#include <thread>
#include "frameman.h"
#include "metaman.h"

using namespace std;

int main() {
    cout << "--------------------------------------------------------------------------" << endl;
    cout << "AXIOM RVCF Emulation - disk benchmarking" << endl;

    cout << "Spawning threads" << endl;

    thread frameThread(RawStreamHandler::FrameManEntry);

    frameThread.join();
//    thread frameThread(RawStreamHandler::FrameManThreadEntry, &buffer);
//    thread metaThread(RawStreamHandler::MetaManThreadEntry, &buffer);
//
//    frameThread.join();
//    metaThread.join();


    cout << "Emulation ends here" << endl;
    cout << "--------------------------------------------------------------------------" << endl;

    return 0;
}