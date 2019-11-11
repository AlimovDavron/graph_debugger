#include <bits/stdc++.h>
#include "json.hpp"
#include <pybind11/embed.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "GDB_MI_Translator.h"

namespace py = pybind11;

using namespace std;

using json = nlohmann::json;

const std::string FIFO_FILE = "graph_debugger";




int main(int argc, char** argv) {
    mknod(FIFO_FILE.c_str(), S_IFIFO | 0666, 0);
    FILE *gdb;
    int fd;
    gdb = popen(("gdb --interpreter=mi --silent > "+FIFO_FILE).c_str(), "w");
    fd = open(FIFO_FILE.c_str(), O_RDONLY);

    auto* translator = new GDB_MI_Translator(gdb, fd);
    cout << argc << endl;
    json result = translator->help();
    cout << result << endl;
    /*json result = translator->setTarget("target");
    cout << result << endl;
    result = translator->setBreakpoint("main");
    cout << result << endl;*/
    /*result = translator->run();
    cout << result << endl;
    result = translator->setWatch("graph");*/

    remove(FIFO_FILE.c_str());
    pclose(gdb);
    return 0;
}