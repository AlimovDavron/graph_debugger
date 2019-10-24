cmake_minimum_required(VERSION 3.15)
project(core)

set(CMAKE_CXX_STANDARD 14)

add_executable(core main.cpp GDB_MI_Interactor.cpp GDB_MI_Interactor.h target.cpp)