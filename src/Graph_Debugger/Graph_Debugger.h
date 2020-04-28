//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPH_DEBUGGER_H
#define CORE_GRAPH_DEBUGGER_H

#include <iostream>
#include "../GDB_Translator/GDB_MI_Translator.h"
#include "GraphDebuggerExceptions.h"
#include "../common/structures.h"

class GraphDebugger {
private:
    GDB_MI_Translator* translator;
    FILE* gdb;
    std::string graphVariableName = "";
    int numberOfVertices = -1;
    vector<std::string> vertexLoads;
    bool targetIsSet;

    bool isVariableInLocals(std::string variableName);
    std::string getAddressOfVariable(const std::string& variableName);
    std::string getValueByAddress(std::string address, int offset, int count);
    std::vector<std::string> getValuesByAddress(std::string address, int n = 1, int count = 8);
    Graph getGraph();
    Position getCurrentPosition();
    std::string getElementOfArray(const std::string&, int);
    void handleMovementResponse(const json&);

public:
    GraphDebugger(int FIFOFileDescriptor, FILE *gdb){
        translator = new GDB_MI_Translator(gdb, FIFOFileDescriptor);
        this->gdb = gdb;
    }

    void setTarget(const std::string& target);
    void start();
    void dump();
    void continue_();
    void next();
    void setGraph(std::string, int);
    void setBkpt(int);
    void debug();
    void attachToVertices(std::string);

};

#endif //CORE_GRAPH_DEBUGGER_H
