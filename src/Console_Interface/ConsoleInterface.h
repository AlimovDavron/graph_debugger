//
// Created by Maruf Karaev on 10/20/19.
//

#ifndef CORE_CONSOLEINTERFACE_H
#define CORE_CONSOLEINTERFACE_H

#include <map>
#include<string>
#include<vector>
#include <iostream>
#include <sstream>
#include <pybind11/embed.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "../../json.hpp"
#include "../Graph_Debugger/Graph_Debugger.h"

using namespace std;
using json = nlohmann::json;


class AbstractCommandHandler {
protected:
    GraphDebugger *graphDebugger;
public:
    explicit AbstractCommandHandler(GraphDebugger* graphDebugger): graphDebugger(graphDebugger) {}
    virtual void execute(istringstream& inputStream) = 0;

    void operator()(istringstream& inputStream) {
        execute(inputStream);
    }

};


class DumpCommandHandler : AbstractCommandHandler {
public:
    explicit DumpCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        cout << "dumping \n";
        Input input = parse_args(inputStream);
        cout << input.path_to_executable_file << " " << input.path_to_graph_data << "\n";
    }

private:
    struct Input {
    public:
        Input(string path_to_executable_file, string path_to_graph_data) :
                path_to_executable_file(path_to_executable_file), path_to_graph_data(path_to_graph_data) {}


        string path_to_executable_file;
        string path_to_graph_data;
    };
    Input parse_args(istringstream& inputStream) {
        string exc_file, path_to_graph_data;
        inputStream >> exc_file >> path_to_graph_data;
        return Input(exc_file, path_to_graph_data);
    }
};

class StepDebugCommandHandler : AbstractCommandHandler {
public:
    explicit StepDebugCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        cout << "dumping \n";
        Input input = parse_args(inputStream);
        cout << input.path_to_executable_file << " " << input.path_to_graph_data << "\n";
    }

private:
    struct Input {
    public:
        Input(string path_to_executable_file, string path_to_graph_data) :
                path_to_executable_file(path_to_executable_file), path_to_graph_data(path_to_graph_data) {}

        string path_to_executable_file;
        string path_to_graph_data;
    };

    Input parse_args(istringstream& inputStream) {
        string exc_file, path_to_graph_data;
        inputStream >> exc_file >> path_to_graph_data;
        return Input(exc_file, path_to_graph_data);
    }
};

class DebugNextCommandHandler : AbstractCommandHandler {
public:
    explicit DebugNextCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        cout << "next command \n";
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class SetTargetCommandHandler : AbstractCommandHandler {
public:
    explicit SetTargetCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string target;
        inputStream >> target;
        this->graphDebugger->setTarget(target);
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class RunCommandHandler : AbstractCommandHandler {
public:
    explicit RunCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        this->graphDebugger->run();
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class SetWatchCommandHandler : AbstractCommandHandler {
public:
    explicit SetWatchCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        cout << "step debugging \n";
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class ExitCommandHandler : AbstractCommandHandler {
public:
    explicit ExitCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& input) override {
        exit(0);
    }
};


class ConsoleInterface {
public:
    ConsoleInterface(int FIFOFileDescriptor, FILE *gdb) {
        auto* graphDebugger = new GraphDebugger(FIFOFileDescriptor, gdb);
        commandsMap = {
                {"exit",       (AbstractCommandHandler*) new ExitCommandHandler(graphDebugger)},
                {"dump",       (AbstractCommandHandler*) new DumpCommandHandler(graphDebugger)},
                {"step_debug", (AbstractCommandHandler*) new StepDebugCommandHandler(graphDebugger)},
                {"next",       (AbstractCommandHandler*) new DebugNextCommandHandler(graphDebugger)},
                {"set_watch",  (AbstractCommandHandler*) new SetWatchCommandHandler(graphDebugger)},
                {"set_target", (AbstractCommandHandler*) new SetTargetCommandHandler(graphDebugger)},
                {"run", (AbstractCommandHandler*) new RunCommandHandler(graphDebugger)}
        };
    }

    [[noreturn]] void run();

private:
    map<string, AbstractCommandHandler*> commandsMap;
};

#endif //CORE_CONSOLEINTERFACE_H
