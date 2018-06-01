//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#ifndef GENERATOR_H
#define GENERATOR_H

#include <fstream>
#include <string>
#include "../common/mlv.h"

using namespace std;

void Populate(mlv_vidf_hdr_t*);
void Populate(mlv_rawi_hdr_t*);
void Populate(mlv_file_hdr_t*);
void Populate(mlv_lens_hdr_t*);
void Populate(mlv_expo_hdr_t*);

char* Zeros(int size);
#endif

// cout << "\r\tProgress: ";