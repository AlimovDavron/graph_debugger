#include <bits/stdc++.h>
#include "json.hpp"
#include <pybind11/embed.h> // everything needed for embedding
namespace py = pybind11;

using namespace std;

using json = nlohmann::json;

string sample = R"(^done,bkpt={number="1",type="breakpoint",disp="keep", enabled="y",addr="0x08048564",func="main",file="myprog.c",fullname="/home/myprog.c",line="68",thread-groups=["i1"],times="0")";

int main(int argc, char *argv[])
{
    py::scoped_interpreter guard{};
    py::module gdbmiparser = py::module::import("pygdbmi.gdbmiparser");

    py::object parsedResponse = py::module::import("json")
            .attr("dumps")(gdbmiparser.attr("parse_response")(sample));

    json a = json::parse(parsedResponse.cast<string>());
    cout << a << endl;
    /*
     py::exec(R"(
        from pygdbmi import gdbmiparser
        from pprint import pprint
        response = gdbmiparser.parse_response('^done,bkpt={number="1",type="breakpoint",disp="keep", enabled="y",addr="0x08048564",func="main",file="myprog.c",fullname="/home/myprog.c",line="68",thread-groups=["i1"],times="0"')
        pprint(response)
    )");
     */
    return 0;
}

/*int main(int argc, char** argv) {
    FILE *gdb = popen("gdb --interpreter=mi --silent", "w");
    fprintf(gdb, "shell ls\n");
    fprintf(gdb, "file %s\n", argv[1]);
    pclose(gdb);


    return 0;
}*/