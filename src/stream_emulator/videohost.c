//
// Copyright 2018, Supragya Raj
// licensed under MIT License (for RawStreamHandler).
//

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// LibFUSE specific details
// Provides with the file where the dump of channel will be available to access, the pathname and the filename
static const char *filepath = "/metaStream";
static const char *filename = "metaStream";

// Input file location
const char INPUT_FILE[] = "../generator/MetaStream.dat";
uint8_t *metaStreamMem;
uint32_t metaStreamSize;

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi);

void preprocess();

// LibFUSE function references to be utilized
static struct fuse_operations fuse_example_operations = {
        .read = read_callback,
};

void load2memory(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    fseek(file, 0L, SEEK_END);
    metaStreamSize = ftell(file);
    rewind(file);

    metaStreamMem = (uint8_t *) malloc(metaStreamSize);
    fread(metaStreamMem, metaStreamSize, 1, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Begin LibFUSE, register the functions with libfuse by sending function references
    printf("MetaStream emulator: Loading to memory\n");
    load2memory(INPUT_FILE);
    return fuse_main(argc, argv, &fuse_example_operations, NULL);
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi) {

    if (strcmp(path, filepath) == 0) {

        if (offset >= metaStreamSize) {
            return 0;
        }

        if (offset + size > metaStreamSize) {
            memcpy(buf, metaStreamMem + offset, metaStreamSize - offset);
            return metaStreamSize - offset;
        }

        memcpy(buf, metaStreamMem + offset, size);
        return size;
    }

    return -ENOENT;
}