//
// Created by supragya on 12/7/18.
//

#ifndef MLV2DNG_MAIN_H
#define MLV2DNG_MAIN_H

#include "../common/mlv.h"
#include <string>
#include <stdint-gcc.h>

void prepare_frames();

void prepare_meta();

void bitinsert(uint16_t *dst, int position, int depth, uint16_t new_value);

#endif //MLV2DNG_MAIN_H
