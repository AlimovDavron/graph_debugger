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

add_executable(core json.hpp main.cpp PyGDBMIParser.cpp PyGDBMIParser.cpp PyGDBMIParser.h Abstract_GDB_MI_Translator.h
        Abstract_GDB_MI_Output_Parser.h GDB_MI_Translator.cpp GDB_MI_Translator.h ArgumentParser.cpp ArgumentParser.h
        ConsoleInterface.cpp ConsoleInterface.h)

target_link_libraries(core PRIVATE pybind11::embed)
