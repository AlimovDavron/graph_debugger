#include <bits/stdc++.h>
#include "json.hpp"
#include <pybind11/embed.h> // everything needed for embedding
namespace py = pybind11;

using namespace std;

using json = nlohmann::json;

int main(int argc, char** argv) {
    FILE *gdb = popen("gdb --interpreter=mi --silent", "w");
    FILE* gdbr = popen(fd[0], "r");

    fprintf(gdb, "file %s\n", argv[1]);
    fflush(write_file);
    fscanf(read_file, "%d", &x);
    pclose(gdb);


    return 0;
}