//
// Created by supragya on 23/7/18.
//


#include "mlv_filler.h"

void popl_file(mlv_file_hdr_t *b) {
    addstr(b->fileMagic, "MLVI", 4);
    b->blockSize = 52;
    addstr(b->versionString, "v2.0", 4);
    b->fileGuid = 3107363179366329747;
    b->fileNum = 0;
    b->fileCount = 0;
    b->fileFlags = 1;
    b->videoClass = 1;
    b->audioClass = 0;
    b->videoFrameCount = 1;
    b->audioFrameCount = 0;
    b->sourceFpsDenom = 1000;
    b->sourceFpsNom = 25000;
}

void popl_rawi(mlv_rawi_hdr_t *b) {
    addstr(b->blockType, "RAWI", 4);
    b->blockSize = sizeof(mlv_rawi_hdr_t);
    b->timestamp = 0;
    b->xRes = 4096;
    b->yRes = 3072;
    popl_rawinfo(&b->raw_info);
}

void popl_rtci(mlv_rtci_hdr_t *b) {
    addstr(b->blockType, "RTCI", 4);
    b->blockSize = sizeof(mlv_rtci_hdr_t);
    b->timestamp = 0;
    b->tm_sec = 4;
    b->tm_min = 55;
    b->tm_hour = 4;
    b->tm_mday = 8;
    b->tm_mon = 6;
    b->tm_year = 118;
    b->tm_wday = 0;
    b->tm_yday = 188;
    b->tm_isdst = 0;
    b->tm_gmtoff = 0;
    addstr(b->tm_zone, "GRNWICH", 8);
}

void popl_rawinfo(raw_info_t *b) {
    b->api_version = 1;
    b->height = 3072;
    b->width = 4096;
    b->pitch = 4096 * 12 / 8;
    b->frame_size = VIDFRAMESIZE;
    b->bits_per_pixel = 12;
    b->black_level = 250;
    b->white_level = 10000;
    b->crop.origin[0] = 0;
    b->crop.origin[1] = 0;
    b->crop.size[0] = 4096;
    b->crop.size[1] = 3072;
    b->active_area.x1 = 0;
    b->active_area.x2 = 4096;
    b->active_area.y1 = 0;
    b->active_area.y2 = 3072;
    b->cfa_pattern = 0x02010100;
    b->dynamic_range = 13;
    int32_t colorMatrix1_4DM3[] = {100, 10000, -1413,
                                   10000, -600, 10000,
                                   -3631, 10000, 11150,
                                   10000, 2850, 10000,
                                   -382, 10000, 1335,
                                   10000, 6437, 10000};
    for (int i = 0; i < 18; i++)
        b->color_matrix1[i] = colorMatrix1_4DM3[i];
}

void popl_vidf(mlv_vidf_hdr_t *b) {
    addstr(b->blockType, "VIDF", 4);
    b->blockSize = sizeof(mlv_vidf_hdr_t) + VIDFRAMESIZE;
    b->timestamp = 0;
    b->frameNumber = 0;
    b->cropPosX = b->cropPosY = 0;
    b->panPosX = 0;
    b->panPosY = 0;
    b->frameSpace = 0;
}

void popl_expo(mlv_expo_hdr_t *b) {
    addstr(b->blockType, "EXPO", 4);
    b->blockSize = sizeof(mlv_expo_hdr_t);
    b->timestamp = 0;
    b->isoMode = 0;
    b->isoValue = 400;
    b->isoAnalog = 88;
    b->digitalGain = 0;
    b->shutterValue = 21348;
}

void popl_idnt(mlv_idnt_hdr_t *b) {
    addstr(b->blockType, "IDNT", 4);
    addstr(b->cameraName, "Apertus Axiom Beta", 18);
    addstr(b->cameraSerial, "00000000", 8);
    b->blockSize = sizeof(mlv_idnt_hdr_t);
    b->timestamp = 0;
    b->cameraModel = 0;
}

void popl_lens(mlv_lens_hdr_t *b) {
    addstr(b->blockType, "LENS", 4);
    b->blockSize = sizeof(mlv_lens_hdr_t);
    b->timestamp = 0;
    b->focalLength = 105;
    b->focalDist = 70;
    b->aperture = 4;
    b->stabilizerMode = 0;
    b->autofocusMode = 1;
    b->flags = 0;
    addstr(b->lensName, "EF24-105mm f/4L IS USM", 23);
    addstr(b->lensSerial, "None", 5);
}

void popl_wbal(mlv_wbal_hdr_t *b) {
    addstr(b->blockType, "WBAL", 4);
    b->blockSize = sizeof(mlv_wbal_hdr_t);
    b->timestamp = 0;
    b->wb_mode = 1;
    b->kelvin = 5200;
    b->wbgain_r = 200;
    b->wbgain_g = 300;
    b->wbgain_b = 200;
    b->wbs_gm = 0;
    b->wbs_ba = 1;
}

void addstr(uint8_t *beg, std::string s, int len) {
    for (int i = 0; i < len; i++, beg++)
        *beg = s[i];
}
