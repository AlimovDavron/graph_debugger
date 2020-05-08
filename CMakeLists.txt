cmake_minimum_required(VERSION 3.10)
project(core)

set(CMAKE_CXX_STANDARD 11)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)

file(COPY pygdbmi
              DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(core json.hpp main.cpp src/GDB_Translator/PyGDBMIParser.cpp src/GDB_Translator/PyGDBMIParser.cpp src/GDB_Translator/PyGDBMIParser.h
        src/GDB_Translator/Abstract_GDB_MI_Output_Parser.h src/GDB_Translator/GDB_MI_Translator.cpp src/GDB_Translator/GDB_MI_Translator.h src/Console_Interface/ArgumentParser.cpp src/Console_Interface/ArgumentParser.h
        src/Console_Interface/ConsoleInterface.cpp src/Console_Interface/ConsoleInterface.h src/GraphDebugger/GraphDebugger.h src/GraphDebugger/GraphDebugger.cpp src/GraphDebugger/GraphDebuggerExceptions.h src/utils/responseUtils.h src/common/structures.h src/Console_Interface/ConsoleInterfaceExceptions.h)

target_link_libraries(core PRIVATE pybind11::embed)
