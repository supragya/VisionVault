
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
const char INPUT_FILE[] = "MetaStream.dat";
uint8_t *metaStreamMem;
uint32_t metaStreamSize;

// Function declarations (nothing much to look here)
static int getattr_callback(const char *path, struct stat *stbuf);

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi);

static int open_callback(const char *path, struct fuse_file_info *fi);


static int read_callback(const char *path, char *buf, size_t size, off_t offset,
                         struct fuse_file_info *fi);

void preprocess();

// LibFUSE function references to be utilized
static struct fuse_operations fuse_example_operations = {
        .getattr = getattr_callback,
        .open = open_callback,
        .read = read_callback,
        .readdir = readdir_callback,
};

void load2memory(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    fseek(file, 0L, SEEK_END);
    long metaStreamSize = ftell(file);
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


// LibFUSE specific functions

// The getattr callback is in charge of reading the metadata of a given path
// this callback is always called before any operation made on the filesystem.
// What we are doing here is simple:

// if the value of path equals to root /, we declare it as a directory and return.
// if the value of path equals to filepath /file, we declare it as a file and explicit its size and then return.
// Otherwise nothing exists at the given path, and we return -ENOENT.
//
// As you can see, we are telling FUSE that the current entry is a file or a directory using the stat struct.
//
// In general, if the entry is a directory, st_mode have to be set to S_IFDIR and st_nlink to 2, while if it’s a file
// st_mode have to be set to S_IFREG (that stands for regular file) and st_nlink to 1.
// Files also require that the st_size (the full file size) is specified.


static int getattr_callback(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    if (strcmp(path, filepath) == 0) {
        stbuf->st_mode = S_IFREG | 0777;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0;
        return 0;
    }

    return -ENOENT;
}

// The readdir callback has the task of telling FUSE the exact structure of the accessed directory.
// Since at the moment the only available directory is /, this function always return its representation,
// we are doing it by filling buf with the two links for the upper directory ..
// and current directory . and with the only file we have: redbytedump

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
                            off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    filler(buf, filename, NULL, 0);

    return 0;
}

// The open callback is called when the system requests for a file to be opened.
// Since we don’t have real file but only in-memory representations,
// we are going to implement this callback just because is needed for FUSE to work and therefore return 0.

static int open_callback(const char *path, struct fuse_file_info *fi) {
    return 0;
}