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

    return 0;
}