cmake_minimum_required(VERSION 3.1)
project(StreamEmulator)

set(CMAKE_C_FLAGS "-D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread")

add_executable(Framestream_emulator metahost.c)
add_executable(Metastream_emulator videohost.c)