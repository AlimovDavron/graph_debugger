//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPHDEBUGGER_H
#define CORE_GRAPHDEBUGGER_H

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
    std::set<EdgeLoad> edgeLoads;
    bool targetIsSet, isStarted;
    std::set<int> beingWatchedVertices;
    std::set<pair<int,int>> beingWatchedEdges;
    std::map<int, VertexWatch> vertexByWatchId;
    std::map<std::string, std::vector<int>> watchIdByVertex;
    std::map<int, EdgeWatch> edgeByWatchId;
    std::map<std::string, std::vector<vector<int>>> watchIdByEdge;

    bool checkTarget();
    bool checkStart();
    bool checkGraph();
    std::string getAddressOfVariable(const std::string& variableName);
    std::string getValueOfVariable(const std::string& variableName);
    std::string getValueByAddress(std::string address, int offset, int count);
    std::vector<std::string> getValuesByAddress(std::string address, int n = 1, int count = 8);
    Graph getGraph();
    std::string getTypeOfVariable(const std::string&);
    Position getCurrentPosition();
    std::string getElementOfArray(const VertexLoad&, int);
    std::string getElementOfEdgeArray(const EdgeLoad& load, int u, int v);
    json handleMovementResponses(std::vector<json>&);
    VertexWatchChanges getVertexWatchChanges(const json& response);
    EdgeWatchChanges getEdgeWatchChanges(const json& response);
    void removeWatch(const int& watchId);
    void setWatchOnVertexHandler(int);
    void setWatchOnEdgeHandler(int, int);
    void removeWatchFromVertexHandler(int);
    json continueHandler();
    json nextHandler();
    json startHandler();
    json detachFromVerticesHandler(std::string);
    json detachFromEdgesHandler(std::string);
    void removeWatchFromEdgeHandler(int, int);

public:
    GraphDebugger(int FIFOFileDescriptor, FILE *gdb){
        translator = new GDB_MI_Translator(gdb, FIFOFileDescriptor);
        this->gdb = gdb;
    }

    void setTarget(const std::string& target);
    void start();
    void dump(std::string);
    void continue_();
    void next();
    void setGraph(std::string, int);
    void setBkpt(int);
    void attachToVertices(std::string);
    void attachToEdges(std::string);
    void detachFromVertices(std::string);
    void detachFromEdges(std::string);
    void setWatchOnVertex(int);
    void setWatchOnEdge(int, int);
    void removeWatchFromVertex(int);
    void removeWatchFromEdge(int, int);

};

#endif //CORE_GRAPHDEBUGGER_H
