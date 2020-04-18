#include <iostream>
#include "json.hpp"
#include <pybind11/embed.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "src/Console_Interface/ConsoleInterface.h"

namespace py = pybind11;

using namespace std;

const std::string FIFO_FILE = "graph_debugger";

int main(int argc, char** argv) {

    mknod(FIFO_FILE.c_str(), S_IFIFO | 0666, 0);
    FILE *gdb;
    int fd;
    gdb = popen(("gdb --interpreter=mi --silent > "+FIFO_FILE).c_str(), "w");
    fd = open(FIFO_FILE.c_str(), O_RDONLY);

    auto* interface = new ConsoleInterface(fd, gdb);
    interface->run();

    // todo: add following lines to exit command of interface
    remove(FIFO_FILE.c_str());
    pclose(gdb);

    return 0;
}