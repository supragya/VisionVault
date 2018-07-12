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

void Populatevidf(mlv_vidf_hdr_t *);

void Populaterawi(mlv_rawi_hdr_t *);

void Populatefile(mlv_file_hdr_t *);

void Populatelens(mlv_lens_hdr_t *);

void Populateexpo(mlv_expo_hdr_t *);

void Populaterawinfot(raw_info_t *);

void Populatertci(mlv_rtci_hdr_t *);

char *Zeros(int size);

#endif

// cout << "\r\tProgress: ";