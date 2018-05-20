//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//
// Synopsis: This file contains the stream handler for Control string

#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H


namespace RawStreamHandler::cm {
    struct ControlStruct {
        int ControlWord;
    };

    void CheckControlStatus();

    void ThreadEntry();

}

#endif