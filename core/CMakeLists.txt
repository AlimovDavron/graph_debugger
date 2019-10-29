cmake_minimum_required(VERSION 3.15)
project(core)

set(CMAKE_CXX_STANDARD 14)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)

add_executable(core json.hpp main.cpp GDB_MI_Communicator.cpp GDB_MI_Communicator.h PyGDBMIParser.cpp PyGDBMIParser.cpp PyGDBMIParser.h Abstract_GDB_MI_Translator.h Abstract_GDB_MI_Output_Parser.h GDB_MI_Translator.cpp GDB_MI_Translator.h)
# target_link_libraries(core ${PYTHON_LIBRARIES})
target_link_libraries(core PRIVATE pybind11::embed)
