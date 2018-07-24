//
// Created by supragya on 24/7/18.
//

#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <iostream>
#include <fstream>
#include <memory.h>
#include <thread>
#include "../common/mlv.h"

void high_speed_handler(std::string steamloc, std::string output);

void low_speed_handler(std::string streamloc, std::string output);

#endif //STREAMHANDLER_H
