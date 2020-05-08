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
#include "../GraphDebugger/GraphDebugger.h"
#include "../GraphDebugger/GraphDebuggerExceptions.h"
#include "ConsoleInterfaceExceptions.h"

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
        std::string path;
        inputStream >> path;
        this->graphDebugger->dump(path);
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
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->setTarget(target);
    }
};

class StartCommandHandler : AbstractCommandHandler {
public:
    explicit StartCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        this->graphDebugger->start();
    }
};

class SetGraphCommandHandler : AbstractCommandHandler {
public:
    explicit SetGraphCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string graph;
        int numberOfVertices;
        inputStream >> graph >> numberOfVertices;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
else
        this->graphDebugger->setGraph(graph, numberOfVertices);
    }
};

class AttachToVerticesCommandHandler : AbstractCommandHandler {
public:
    explicit AttachToVerticesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->attachToVertices(array);
    }
};

class AttachToEdgesCommandHandler : AbstractCommandHandler {
public:
    explicit AttachToEdgesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->attachToEdges(array);
    }
};

class DetachFromVerticesCommandHandler : AbstractCommandHandler {
public:
    explicit DetachFromVerticesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->detachFromVertices(array);
    }
};

class DetachFromEdgesCommandHandler : AbstractCommandHandler {
public:
    explicit DetachFromEdgesCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        std::string array;
        inputStream >> array;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->detachFromEdges(array);
    }
};


class SetBkptCommandHandler : AbstractCommandHandler {
public:
    explicit SetBkptCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int lineNumber;
        inputStream >> lineNumber;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->setBkpt(lineNumber);
    }
};


class SetWatchOnVertexCommandHandler : AbstractCommandHandler {
public:
    explicit SetWatchOnVertexCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int vertexIndex;
        inputStream >> vertexIndex;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->setWatchOnVertex(vertexIndex);
    }
};

class SetWatchOnEdgeCommandHandler : AbstractCommandHandler {
public:
    explicit SetWatchOnEdgeCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int from, to;
        inputStream >> from >> to;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->setWatchOnEdge(from, to);
    }
};

class RemoveWatchFromVertexCommandHandler : AbstractCommandHandler {
public:
    explicit RemoveWatchFromVertexCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int vertexIndex;
        inputStream >> vertexIndex;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->removeWatchFromVertex(vertexIndex);
    }
};

class RemoveWatchFromEdgeCommandHandler : AbstractCommandHandler {
public:
    explicit RemoveWatchFromEdgeCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& inputStream) override {
        int from, to;
        inputStream >> from >> to;
        if(inputStream.fail())
            throw InputErrorException(json({{"success", false}, {"error", "wrong input"}}).dump());
        else this->graphDebugger->removeWatchFromEdge(from, to);
    }
};


class ExitCommandHandler : AbstractCommandHandler {
public:
    explicit ExitCommandHandler(GraphDebugger* graphDebugger) : AbstractCommandHandler(graphDebugger) {}
    void execute(istringstream& input) override {
        throw ExitException(json({{"success", true}}).dump());
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
                {"set-watch-on-edge", (AbstractCommandHandler*) new SetWatchOnEdgeCommandHandler(graphDebugger)},
                {"remove-watch-from-vertex", (AbstractCommandHandler*) new RemoveWatchFromVertexCommandHandler(graphDebugger)},
                {"remove-watch-from-edge", (AbstractCommandHandler*) new RemoveWatchFromEdgeCommandHandler(graphDebugger)},
                {"set-target", (AbstractCommandHandler*) new SetTargetCommandHandler(graphDebugger)},
                {"start",      (AbstractCommandHandler*) new StartCommandHandler(graphDebugger)},
                {"set-graph",  (AbstractCommandHandler*) new SetGraphCommandHandler(graphDebugger)},
                {"set-bkpt", (AbstractCommandHandler*) new SetBkptCommandHandler(graphDebugger)},
                {"attach-to-vertices", (AbstractCommandHandler*) new AttachToVerticesCommandHandler(graphDebugger)},
                {"attach-to-edges", (AbstractCommandHandler*) new AttachToEdgesCommandHandler(graphDebugger)},
                {"detach-from-vertices", (AbstractCommandHandler*) new DetachFromVerticesCommandHandler(graphDebugger)},
                {"detach-from-edges", (AbstractCommandHandler*) new DetachFromEdgesCommandHandler(graphDebugger)}
        };
    }

    [[noreturn]] void run();

    void stop();

private:
    map<string, AbstractCommandHandler*> commandsMap;
    FILE *gdb;
};

#endif //CORE_CONSOLEINTERFACE_H
