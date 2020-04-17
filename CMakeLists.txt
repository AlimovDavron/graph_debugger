cmake_minimum_required(VERSION 3.15)
project(core)

set(CMAKE_CXX_STANDARD 11)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)

add_executable(target target.cpp)
configure_file(graph graph COPYONLY)
file(COPY pygdbmi
              DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(core json.hpp main.cpp src/GDB_Translator/PyGDBMIParser.cpp src/GDB_Translator/PyGDBMIParser.cpp src/GDB_Translator/PyGDBMIParser.h
        src/GDB_Translator/Abstract_GDB_MI_Output_Parser.h src/GDB_Translator/GDB_MI_Translator.cpp src/GDB_Translator/GDB_MI_Translator.h src/Console_Interface/ArgumentParser.cpp src/Console_Interface/ArgumentParser.h
        src/Console_Interface/ConsoleInterface.cpp src/Console_Interface/ConsoleInterface.h src/Graph_Debugger/Graph_Debugger.h src/Graph_Debugger/Graph_Debugger.cpp)

target_link_libraries(core PRIVATE pybind11::embed)
