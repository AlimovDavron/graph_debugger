//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPH_DEBUGGER_H
#define CORE_GRAPH_DEBUGGER_H

#include <iostream>
#include <set>
#include "../GDB_Translator/GDB_MI_Translator.h"
#include "GraphDebuggerExceptions.h"
#include "../common/structures.h"

class GraphDebugger {
private:
    GDB_MI_Translator* translator;
    FILE* gdb;
    std::string graphVariableName;
    std::string addressOfVariable;
    int numberOfVertices = -1;
    std::set<VertexLoad> vertexLoads;
    bool targetIsSet;
    std::set<int> beingWatchedVertices;
    std::map<int, VertexWatch> vertexByWatchId;
    std::map<std::string, std::vector<int>> watchIdByVertex;

    bool isVariableInLocals(std::string variableName);
    std::string getAddressOfVariable(const std::string& variableName);
    std::string getValueOfVariable(const std::string& variableName);
    std::string getValueByAddress(std::string address, int offset, int count);
    std::vector<std::string> getValuesByAddress(std::string address, int n = 1, int count = 8);
    Graph getGraph();
    std::string getTypeOfVariable(const std::string&);
    Position getCurrentPosition();
    std::string getElementOfArray(const VertexLoad&, int);
    json handleMovementResponses(std::vector<json>&);
    WatchChanges getWatchChanges(const json& response);
    void removeWatch(const int& watchId);
    void setWatchOnVertexHandler(int);
    void removeWatchFromVertexHandler(int);
    json continueHandler();
    json nextHandler();
    json startHandler();

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
    void detachFromVertices(std::string);
    void setWatchOnVertex(int);
    void removeWatchFromVertex(int);

};

#endif //CORE_GRAPH_DEBUGGER_H
