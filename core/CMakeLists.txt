cmake_minimum_required(VERSION 3.15)
project(core)

set(CMAKE_CXX_STANDARD 14)

add_executable(core main.cpp GDB_MI_Communicator.cpp GDB_MI_Communicator.h PyGDBMIParser.cpp PyGDBMIParser.cpp PyGDBMIParser.h Abstract_GDB_MI_Communicator.h Abstract_GDB_MI_Output_Parser.h)