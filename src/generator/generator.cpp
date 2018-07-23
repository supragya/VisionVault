#include <iostream>
#include <fstream>
#include <vector>
#include "generator.h"
#include "mlv_filler.h"

#define FRAMECOUNT 3

using namespace std;

int main() {
    cout << "Begin preparing meta stream (low speed)" << endl;
    prepare_meta();
    cout << "End preparing meta stream (low speed) - done" << endl;

    cout << "Begin preparing videoframe stream (high speed)" << endl;
    prepare_frames();
    cout << "End preparing videoframe stream (high speed)" << endl;
    return 0;
}

void prepare_frames() {
    string raw12_frame[FRAMECOUNT] = {"../camera_internals/frame/Intern.raw12",
                                      "../camera_internals/frame/ClippedWall.raw12",
                                      "../camera_internals/frame/IT8Chart15ms.raw12"};
    string high_speed_data = "../processed_data/cam_framedata.rawdata";
    ofstream output(high_speed_data, ios::binary | ios::trunc | ios::out);

    if (!output.is_open()) {
        cerr << "Error: Cannot open the following file(s): ";
        cerr <<  high_speed_data << endl;
        cerr << "Generator exiting. Advised rerun after \"mkdir processed_data\"" << endl;
        exit(1);
    }

    mlv_vidf_hdr_t vidf_hdr = {0};
    popl_vidf(&vidf_hdr);

    auto *raw = new uint8_t[VIDFRAMESIZE];
    auto *list_of_values = new uint16_t[4096 * 3072];
    auto *dst = new uint16_t[VIDFRAMESIZE / 2];

    for (int i = 0; i < FRAMECOUNT; i++) {
        cout << "Frame: Reading to encode: " << raw12_frame[i] << endl;
        ifstream raw12file(raw12_frame[i], ios::in | ios::binary);

        if (!raw12file.is_open()) {
            cerr << "Error: Cannot open the following file(s): ";
            cerr << raw12_frame[i] << endl;
            cerr << "Generator exiting. Check if this file is accessible." << endl;
            exit(1);
        }

        raw12file.read((char *) raw, VIDFRAMESIZE);
        raw12file.close();

        int pos_list = 0, pos_raw = 0;
        for (pos_raw = 0; pos_raw < 4096 * 3072 * 12 / 8; pos_raw += 3) {
            list_of_values[pos_list] = ((uint16_t) raw[pos_raw] << 4) | (raw[pos_raw + 1] & 0xF0) >> 4;
            pos_list++;
            list_of_values[pos_list] = ((uint16_t) (raw[pos_raw + 1] & 0x0F) << 8) | (raw[pos_raw + 2]);
            pos_list++;
        }

        for (int j = 0; j < 4092 * 3072; j++)
            bitinsert(dst, j, 12, list_of_values[i]);

        output.write(reinterpret_cast<char *>(&vidf_hdr), sizeof(vidf_hdr));
        output.write(reinterpret_cast<char *>(dst), VIDFRAMESIZE);
    }

    output.close();
    delete raw;
    delete list_of_values;
    delete dst;
}

void prepare_meta() {
    string low_speed_data = "../processed_data/cam_metadata.rawinfo";
    ofstream output(low_speed_data, ios::binary | ios::out);

    if (!output.is_open()) {
        cerr << "Error: Cannot open the following file(s): ";
        cerr << low_speed_data << endl;
        cerr << "Generator exiting. Advised rerun after \"mkdir processed_data\"" << endl;
        exit(1);
    }

    mlv_file_hdr_t file_hdr = {0};
    mlv_rawi_hdr_t rawi_hdr = {0};
    mlv_rtci_hdr_t rtci_hdr = {0};
    mlv_expo_hdr_t expo_hdr = {0};
    mlv_idnt_hdr_t idnt_hdr = {0};
    mlv_lens_hdr_t lens_hdr = {0};
    mlv_wbal_hdr_t wbal_hdr = {0};

    popl_file(&file_hdr);
    popl_rawi(&rawi_hdr);
    popl_rtci(&rtci_hdr);
    popl_expo(&expo_hdr);
    popl_idnt(&idnt_hdr);
    popl_lens(&lens_hdr);
    popl_wbal(&wbal_hdr);

    output.write(reinterpret_cast<char *>(&file_hdr), sizeof(file_hdr));
    output.write(reinterpret_cast<char *>(&rawi_hdr), sizeof(rawi_hdr));
    output.write(reinterpret_cast<char *>(&rtci_hdr), sizeof(rtci_hdr));
    output.write(reinterpret_cast<char *>(&expo_hdr), sizeof(expo_hdr));
    output.write(reinterpret_cast<char *>(&idnt_hdr), sizeof(idnt_hdr));
    output.write(reinterpret_cast<char *>(&lens_hdr), sizeof(lens_hdr));
    output.write(reinterpret_cast<char *>(&wbal_hdr), sizeof(wbal_hdr));

    output.close();
}

void bitinsert(uint16_t *dst, int position, int depth, uint16_t new_value) {
    uint16_t old_value = 0;
    int dst_pos = position * depth / 16;
    int bits_to_left = ((depth * position) - (16 * dst_pos)) % 16;
    int shift_right = 16 - depth - bits_to_left;

    old_value = dst[dst_pos];
    if (shift_right >= 0) {
        /* this case is a bit simpler. the word fits into this uint16_t */
        uint16_t mask = ((1 << depth) - 1) << shift_right;

        /* shift and mask out */
        new_value <<= shift_right;
        new_value &= mask;
        old_value &= ~mask;

        /* now combine */
        new_value |= old_value;
        dst[dst_pos] = new_value;
    } else {
        /* here we need two operations as the bits are split over two words */
        uint16_t mask1 = ((1 << (depth + shift_right)) - 1);
        uint16_t mask2 = ((1 << (-shift_right)) - 1) << (16 + shift_right);

        /* write the upper bits */
        old_value &= ~mask1;
        old_value |= (new_value >> (-shift_right)) & mask1;
        dst[dst_pos] = old_value;

        /* write the lower bits */
        old_value = dst[dst_pos + 1];
        old_value &= ~mask2;
        old_value |= (new_value << (16 + shift_right)) & mask2;
        dst[dst_pos + 1] = old_value;
    }
}