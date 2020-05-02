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
#include "../Graph_Debugger/GraphDebuggerExceptions.h"

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

class ContinueCommandHandler : AbstractCommandHandler {
public:
    explicit ContinueCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        this->graphDebugger->continue_();
    }
};

class NextCommandHandler : AbstractCommandHandler {
public:
    explicit NextCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        this->graphDebugger->next();
    }

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

class StartCommandHandler : AbstractCommandHandler {
public:
    explicit StartCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        this->graphDebugger->start();
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class SetGraphCommandHandler : AbstractCommandHandler {
public:
    explicit SetGraphCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string graph;
        int numberOfVertices;
        inputStream >> graph >> numberOfVertices;
        this->graphDebugger->setGraph(graph, numberOfVertices);
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class AttachToVerticesCommandHandler : AbstractCommandHandler {
public:
    explicit AttachToVerticesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        this->graphDebugger->attachToVertices(array);
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class DetachFromVerticesCommandHandler : AbstractCommandHandler {
public:
    explicit DetachFromVerticesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        this->graphDebugger->detachFromVertices(array);
    }

private:
    void parse_args(istringstream& inputStream) {}
};

class SetBkptCommandHandler : AbstractCommandHandler {
public:
    explicit SetBkptCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int lineNumber;
        inputStream >> lineNumber;
        this->graphDebugger->setBkpt(lineNumber);
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class SetWatchOnVertexCommandHandler : AbstractCommandHandler {
public:
    explicit SetWatchOnVertexCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int vertexIndex;
        inputStream >> vertexIndex;
        this->graphDebugger->setWatchOnVertexHandler(vertexIndex);
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class ExitCommandHandler : AbstractCommandHandler {
public:
    explicit ExitCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& input) override {
        throw ExitException("user interrupt");
    }
};

class DebugCommandHandler : AbstractCommandHandler {
public:
    explicit DebugCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& input) override {
        this->graphDebugger->debug();
    }
};

const std::string FIFO_FILE = "graph_debugger";

class ConsoleInterface {
public:
    ConsoleInterface(int FIFOFileDescriptor, FILE *gdb) {
        this->gdb = gdb;
        auto* graphDebugger = new GraphDebugger(FIFOFileDescriptor, gdb);
        commandsMap = {
                {"exit",       (AbstractCommandHandler*) new ExitCommandHandler(graphDebugger)},
                {"dump",       (AbstractCommandHandler*) new DumpCommandHandler(graphDebugger)},
                {"continue",   (AbstractCommandHandler*) new ContinueCommandHandler(graphDebugger)},
                {"next",       (AbstractCommandHandler*) new NextCommandHandler(graphDebugger)},
                {"set-watch-on-vertex",  (AbstractCommandHandler*) new SetWatchOnVertexCommandHandler(graphDebugger)},
                {"set-target", (AbstractCommandHandler*) new SetTargetCommandHandler(graphDebugger)},
                {"start",      (AbstractCommandHandler*) new StartCommandHandler(graphDebugger)},
                {"set-graph",  (AbstractCommandHandler*) new SetGraphCommandHandler(graphDebugger)},
                {"set-bkpt", (AbstractCommandHandler*) new SetBkptCommandHandler(graphDebugger)},
                {"attach-to-vertices", (AbstractCommandHandler*) new AttachToVerticesCommandHandler(graphDebugger)},
                {"detach-from-vertices", (AbstractCommandHandler*) new DetachFromVerticesCommandHandler(graphDebugger)},
                //todo: remove this later
                {"debug", (AbstractCommandHandler*) new DebugCommandHandler(graphDebugger)}
                //{"dump",       (AbstractCommandHandler*) new DumpCommandHandler(graphDebugger)},
        };
    }

    [[noreturn]] void run();

    void stop();

private:
    map<string, AbstractCommandHandler*> commandsMap;
    FILE *gdb;
};

#endif //CORE_CONSOLEINTERFACE_H
