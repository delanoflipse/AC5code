cmake_minimum_required(VERSION 3.6)
project(SRC)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c challenge12.c challenge12.h matrix.c matrix.h serial.c serial.h debug.c debug.h challenge3.c challenge3.h)
add_executable(SRC ${SOURCE_FILES})