//
// Created by Maruf Karaev on 10/20/19.
//

#include "ArgumentParser.h"

ArgumentParser::Argument ArgumentParser::parse_arg(char *arg) {
    std::string arg_str = std::string(arg);
    int delimiter = arg_str.find('=');
    return Argument(arg_str.substr(0, delimiter),
                    arg_str.substr(delimiter + 1));

}

std::map<std::string, std::string> ArgumentParser::parse_args(int argc, char **argv) {
    if (argc < 2) {
        throw std::invalid_argument("Invalid arguments count");
    }

    std::map<std::string, std::string> args;
    bool exc_file = false;
    bool graph_data = false;
    for (int i = 1; i < argc; i++) {
        auto arg = parse_arg(argv[i]);
        args[arg.getName()] = arg.getValue();
        if (arg.getName() == "exc_file") {
            exc_file = true;
        }
        if (arg.getName() == "graph_data") {
            graph_data = true;
        }
    }
    if (!exc_file or !graph_data) {
        throw std::invalid_argument("Path to executable file, or path graph data is not passed");
    }
    return args;
}
