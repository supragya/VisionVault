cmake_minimum_required(VERSION 3.1)
project(Generator)

add_executable(Generator generator.cpp generator.h ../common/mlv.h mlv_filler.cpp mlv_filler.h)