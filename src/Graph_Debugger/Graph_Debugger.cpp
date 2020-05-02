//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"
#include "../utils/responseUtils.h"
using namespace responseUtils;

void GraphDebugger::dump() {

}

static void sendResponse(const json& response){
    cout << response << endl;
}

void GraphDebugger::setTarget(const std::string& target) {
    std::vector<json> responses = this->translator->executeCommand("file " + target, 'h');
    this->targetIsSet = true;
    for(const auto& response: responses){
        if(response["type"] == "result"){
            if(response["message"] == "done"){
                sendResponse(responseUtils::createBinaryResponse(
                        true, ""));
            } else {
                sendResponse(responseUtils::createBinaryResponse(
                        false, response["payload"]["msg"]));
            }
        }
    }
}

WatchChanges GraphDebugger::getWatchChanges(const json& response){
    int watchId = (int)stoi((string)response["payload"]["wpt"]["number"], NULL, 10);
    return WatchChanges(response["payload"]["value"]["old"],
                        response["payload"]["value"]["new"],
                        this->vertexByWatchId[watchId].load,
                        this->vertexByWatchId[watchId].vertexIndex);
}

json GraphDebugger::handleMovementResponses(std::vector<json>& responses) {
    for(const auto& response: responses) {
        if (response["type"] == "notify" && response["message"] == "stopped") {

            std::string reason = response["payload"]["reason"];
            if (reason == "signal-received") {
                return responseUtils::createErrorStopResponse(
                        response["payload"]["reason"],
                        response["payload"]["signal-meaning"],
                        getGraph(),
                        getCurrentPosition());
            }

            if (reason == "exited-normally" or reason == "exited-signalled")
                throw ExitException("success");

            if (reason == "watchpoint-trigger") {
                return responseUtils::createWatchTriggerStopResponse(
                        response["payload"]["reason"],
                        getGraph(),
                        getCurrentPosition(),
                        getWatchChanges(response));
            }

            return responseUtils::createStopResponse(
                    response["payload"]["reason"],
                    getGraph(),
                    getCurrentPosition());
        }
    }
}

json GraphDebugger::startHandler() {
    std::vector<json> responses = this->translator->executeCommand("-exec-run --start", 's');
    return handleMovementResponses(responses);
}

void GraphDebugger::start() {
    sendResponse(startHandler());
}

json GraphDebugger::continueHandler() {
    std::vector<json> responses = this->translator->executeCommand("-exec-continue", 's');
    return handleMovementResponses(responses);
}

void GraphDebugger::continue_() {
    sendResponse(continueHandler());
}

json GraphDebugger::nextHandler() {
    std::vector<json> responses = this->translator->executeCommand("next", 's');
    return handleMovementResponses(responses);
}

void GraphDebugger::next() {
    sendResponse(nextHandler());
}

static std::string getVariableName(std::string response){
    size_t pos = response.find(' ');
    if(pos == std::string::npos){
        return response;
    } else {
        return response.substr(0, pos);
    }
}

std::string GraphDebugger::getAddressOfVariable(const std::string& variableName) {
    std::vector<json> responses = this->translator->executeCommand("-data-evaluate-expression &" + variableName, 'h');
    for (const auto &response: responses) {
        if(response["type"] ==  "result"){
            if(response["message"] ==  "done"){
                return response["payload"]["value"];
            }
            // todo: handle error
        }
    }
}

std::string GraphDebugger::getValueOfVariable(const std::string& variableName) {
    std::vector<json> responses = this->translator->executeCommand("-data-evaluate-expression " + variableName, 'h');
    for (const auto &response: responses) {
        if(response["type"] ==  "result"){
            if(response["message"] ==  "done"){
                return response["payload"]["value"];
            }
            // todo: handle error
        }
    }
}

std::string GraphDebugger::getTypeOfVariable(const std::string& variableName) {
    std::vector<json> responses = this->translator->executeCommand("whatis " + variableName, 'h');
    for (const auto &response: responses) {
        if(response["type"] == "console"){
            std::string payload = response["payload"];
            size_t pos_eq = payload.find('='); pos_eq+=2;
            return payload.substr(pos_eq, payload.find(' ', pos_eq) - pos_eq);
        }
    }
}

std::string GraphDebugger::getElementOfArray(const VertexLoad& load, int index) {
    std::vector<json> responses = this->translator->
            executeCommand("-data-evaluate-expression (" +
            load.type + ")*("+ load.address+"+"+std::to_string(index)+"*sizeof(" + load.type+"))", 'h');

    for (const auto &response: responses) {
        if(response["type"] ==  "result"){
            if(response["message"] ==  "done"){
                return response["payload"]["value"];
            }
            // todo: handle error
        }
    }
}

Graph GraphDebugger::getGraph() {
    if(numberOfVertices == -1){
        return Graph();
    }

    std::vector<std::vector<int>> adjacencyMatrix(this->numberOfVertices, vector<int>(this->numberOfVertices));
    map<std::string, std::vector<std::string>> loads;

    std::vector<std::string> pointersToAdjacencyMatrixLines =
            getValuesByAddress(getValueByAddress(this->addressOfVariable, 0, 8), numberOfVertices);

    int i = 0, j = 0;
    for(const auto& pointerToLine: pointersToAdjacencyMatrixLines){
        std::vector<std::string> valuesOfLine = getValuesByAddress(pointerToLine, numberOfVertices, 4);
        for(const auto& value: valuesOfLine){
            adjacencyMatrix[i][j] = ((int)strtol(value.c_str(), NULL, 16));
            j++;
        }
        i++; j = 0;
    }


    for(const auto& load: this->vertexLoads){
        vector<std::string> values(this->numberOfVertices, "");
        for(i = 0; i < this->numberOfVertices; i++){
            values[i] = getElementOfArray(load, i);
        }

        loads[load.variableName] = values;
    }

    return Graph(adjacencyMatrix, loads);
}

void GraphDebugger::setGraph(std::string graph, int n) {
    this->graphVariableName = graph;
    this->numberOfVertices = n;
    this->addressOfVariable = getAddressOfVariable(graph);

    sendResponse(responseUtils::createSetGraphResponse(true, getGraph()));
}

// todo: Danger zone! Following code works only on 64 bit architecture
std::vector<std::string> GraphDebugger::getValuesByAddress(std::string address, int n, int count) {
    std::vector<std::string> result(n);
    for(int i = 0; i < n; i++){
        result[i] = getValueByAddress(address, i*count, count);
    }

    return result;
}
std::string GraphDebugger::getValueByAddress(std::string address, int offset, int count) {
    std::vector<json> responses = this->translator->
            executeCommand("-data-read-memory-bytes -o " + to_string(offset) + " " + address + " " + to_string(count) + " ", 'h');

    std::string result(count*2, ' ');
    for(const auto& response: responses){
        if(response["type"] == "result"){
            std::string value = response["payload"]["memory"][0]["contents"];
            for(int i = 0; i < count; i++){
                result[2*i] = value[2*count - 2*i - 2];
                result[2*i+1] = value[2*count - 2*i - 1];
            }
            return "0x"+result;
        }
        // todo: handle error
    }
}

void GraphDebugger::setBkpt(int lineNumber) {
    std::vector<json> responses = this->translator->
            executeCommand("-break-insert " + std::to_string(lineNumber), 'h');

    for(const auto& response: responses){
        if(response["type"] == "result"){
            sendResponse(responseUtils::createBinaryResponse(response["message"] == "done", response["message"]));
            return;
        }
    }
}

Position GraphDebugger::getCurrentPosition() {
    std::vector<json> responses = this->translator->
            executeCommand("-stack-info-frame", 'h');

    for(const auto& response: responses){
        if(response["type"] == "result"){
            return Position(stoi((string)response["payload"]["frame"]["line"], NULL, 10),
                            response["payload"]["frame"]["file"],
                            response["payload"]["frame"]["func"]);
        }
    }
}


void GraphDebugger::attachToVertices(std::string variableName) {
    std::string address = getValueOfVariable(variableName);
    std::string type = getTypeOfVariable(variableName);
    this->vertexLoads.insert(VertexLoad(variableName,
                             address,
                             type));

    this->watchIdByVertex[variableName] = std::vector<int>(this->numberOfVertices, -1);

    for(const auto& vertexIndex: this->beingWatchedVertices) {
        std::vector<json> responses = this->translator->
                executeCommand("watch *(" + type + " *) (" + address + "+" + to_string(vertexIndex) + "*sizeof(" + type + "))", 'h');

        for(const auto& response: responses){
            if(response["type"] == "notify"){
                int watchId = (int)stoi((string)response["payload"]["bkpt"]["number"], NULL, 10);
                this->vertexByWatchId[watchId] = {variableName, vertexIndex};
                this->watchIdByVertex[variableName][vertexIndex] = watchId;
            }
        }
    }

    sendResponse(responseUtils::createSetGraphResponse(true, getGraph()));
}

void GraphDebugger::removeWatch(const int &watchId) {
    std::vector<json> responses = this->translator-> executeCommand("delete " + std::to_string(watchId), 'h');
}


void GraphDebugger::detachFromVertices(std::string variableName) {
    this->vertexLoads.erase(VertexLoad(variableName, "", ""));

    for(const auto& watchId: this->watchIdByVertex[variableName]){
        if(watchId == -1)
            continue;

        this->vertexByWatchId.erase(watchId);
        removeWatch(watchId);
    }

    sendResponse(responseUtils::createSetGraphResponse(true, getGraph()));
}

void GraphDebugger::setWatchOnVertexHandler(int vertexIndex){
    this->beingWatchedVertices.insert(vertexIndex);

    for(const auto& load: this->vertexLoads) {
        std::vector<json> responses = this->translator->
                executeCommand("watch *(" + load.type + " *) (" + load.address + "+" + to_string(vertexIndex) + "*sizeof(" + load.type + "))", 'h');

        for(const auto& response: responses){
            if(response["type"] == "notify"){
                int watchId = (int)stoi((string)response["payload"]["bkpt"]["number"], NULL, 10);
                this->vertexByWatchId[watchId] = VertexWatch(load.variableName, vertexIndex);
                this->watchIdByVertex[load.variableName][vertexIndex] = watchId;
            }
        }
    }
}

void GraphDebugger::setWatchOnVertex(int vertexIndex) {
    setWatchOnVertex(vertexIndex);

    sendResponse(responseUtils::createBinaryResponse(true, "watchpoint is set"));
}


// todo: remove this later
void GraphDebugger::debug() {
    std::vector<json> responses = this->translator->
            executeCommand("-break-insert 12312312312312312", 'h');

    for(const auto& response: responses){
        cout << response << endl;
    }
}










