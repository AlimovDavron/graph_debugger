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

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

string readChunk(){
    int size;
    char buf[4096];
    string result = "";
    while((size = read(fd, buf, sizeof(buf) - 1)) > 0){
        buf[size] = '\0';
        result += buf;
        if(has_suffix(result, "(gdb) \n")){
            return result;
        }
    }
}

void initConnection(){
    gdb = popen("gdb --interpreter=mi --silent > graph_debugger", "w");
    fd = open(FIFO, O_RDONLY);
    string result = readChunk();
}

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