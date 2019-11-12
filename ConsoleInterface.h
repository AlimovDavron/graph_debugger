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

using namespace std;


class AbstractCommandHandler {
public:
    virtual void execute(istringstream& inputStream) = 0;

    void operator()(istringstream& inputStream) {
        execute(inputStream);
    }

};


class DumpCommandHandler : AbstractCommandHandler {
public:
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
    void execute(istringstream& inputStream) override {
        cout << "next command \n";
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class SetWatchCommandHandler : AbstractCommandHandler {
public:
    void execute(istringstream& inputStream) override {
        cout << "step debugging \n";
    }

private:
    void parse_args(istringstream& inputStream) {}
};


class ExitCommandHandler : AbstractCommandHandler {
public:
    void execute(istringstream& input) override {
        exit(0);
    }
};


class ConsoleInterface {
public:
    ConsoleInterface() {
        commandsMap = {
                {"exit",       (AbstractCommandHandler*) new ExitCommandHandler},
                {"dump",       (AbstractCommandHandler*) new DumpCommandHandler},
                {"step_debug", (AbstractCommandHandler*) new StepDebugCommandHandler},
                {"next",       (AbstractCommandHandler*) new DebugNextCommandHandler},
                {"set_watch",  (AbstractCommandHandler*) new SetWatchCommandHandler},
        };
    }

    void run();

private:
    map<string, AbstractCommandHandler*> commandsMap;
};

#endif //CORE_CONSOLEINTERFACE_H
