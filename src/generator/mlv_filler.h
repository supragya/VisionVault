//
// Created by supragya on 23/7/18.
//

#ifndef RVCF_EMULATION_MLV_FILLER_H
#define RVCF_EMULATION_MLV_FILLER_H

#include <string>
#include "../common/mlv.h"

#define VIDFRAMESIZE (4096*3072*12/8)

void popl_file(mlv_file_hdr_t *);

void popl_rawi(mlv_rawi_hdr_t *);

void popl_rtci(mlv_rtci_hdr_t *);

void popl_vidf(mlv_vidf_hdr_t *);

void popl_expo(mlv_expo_hdr_t *);

void popl_idnt(mlv_idnt_hdr_t *);

void popl_lens(mlv_lens_hdr_t *);

void popl_wbal(mlv_wbal_hdr_t *);

void popl_rawinfo(raw_info_t *);

void addstr(uint8_t *beg, std::string s, int len);


#endif //RVCF_EMULATION_MLV_FILLER_H
