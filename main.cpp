#include <bits/stdc++.h>
#include "json.hpp"
#include <pybind11/embed.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFO "graph_debugger"

namespace py = pybind11;

using namespace std;

using json = nlohmann::json;

FILE *gdb;
int fd;


int main(int argc, char** argv) {
    mknod(FIFO, S_IFIFO | 0666, 0);

    initConnection();

    fprintf(gdb, "file %s\n", argv[1]);
    fflush(gdb);
    string t = readChunk();
    cout << t << endl;

    remove(FIFO);
    pclose(gdb);
    return 0;
}