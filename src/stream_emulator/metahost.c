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
static const char *filepath = "/frameStream";
static const char *filename = "frameStream";

// Input file location
const char INPUT_FILE[] = "../generator/FrameStream.dat";
uint8_t *frameStreamMem;
uint32_t frameStreamSize;

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
    frameStreamSize = ftell(file);
    rewind(file);

    frameStreamMem = (uint8_t *) malloc(frameStreamSize);
    fread(frameStreamMem, frameStreamSize, 1, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Begin LibFUSE, register the functions with libfuse by sending function references
    printf("FrameStream emulator running\n");
    load2memory(INPUT_FILE);
    return fuse_main(argc, argv, &fuse_example_operations, NULL);
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi) {

    if (strcmp(path, filepath) == 0) {

        if (offset >= frameStreamSize) {
            return 0;
        }

        if (offset + size > frameStreamSize) {
            memcpy(buf, frameStreamMem + offset, frameStreamSize - offset);
            return frameStreamSize - offset;
        }

        memcpy(buf, frameStreamMem + offset, size);
        return size;
    }

    return -ENOENT;
}