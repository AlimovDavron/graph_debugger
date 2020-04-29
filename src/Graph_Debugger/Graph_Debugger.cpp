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

int getVertexNumber(const std::string& expression){
    size_t os(expression.find('[')), cs(expression.find(']'));
    return (int)stoi(expression.substr(os+1, cs - os - 1), NULL, 10);
}

std::string getLoad(const std::string& expression){
    return expression.substr(0, expression.find('['));
}

static WatchChanges getWatchChanges(const json& response){
    std::string expression = response["payload"]["wpt"]["exp"];
    return WatchChanges(response["payload"]["value"]["old"],
                        response["payload"]["value"]["new"],
                        getLoad(expression),
                        getVertexNumber(expression));
}

void GraphDebugger::handleMovementResponse(const json & response) {
    if(response["type"] == "notify" && response["message"] == "stopped") {

        std::string reason = response["payload"]["reason"];
        if(reason == "signal-received"){
            sendResponse(responseUtils::createErrorStopResponse(
                    response["payload"]["reason"],
                    response["payload"]["signal-meaning"],
                    getGraph(),
                    getCurrentPosition()));
            return;
        }

        if(reason == "exited-normally" or reason == "exited-signalled")
            throw ExitException("success");

        if(reason == "watchpoint-trigger"){
            sendResponse(responseUtils::createWatchTriggerStopResponse(
                    response["payload"]["reason"],
                    getGraph(),
                    getCurrentPosition(),
                    getWatchChanges(response)));

            return;
        }

        sendResponse(responseUtils::createStopResponse(
                response["payload"]["reason"],
                getGraph(),
                getCurrentPosition()));
    }
}

void GraphDebugger::start() {
    std::vector<json> responses = this->translator->executeCommand("-exec-run --start", 's');
    for(const auto& response: responses){
        handleMovementResponse(response);
    }

}

void GraphDebugger::continue_() {
    std::vector<json> responses = this->translator->executeCommand("-exec-continue", 's');
    for(const auto& response: responses){
        handleMovementResponse(response);
    }
}

void GraphDebugger::next() {
    std::vector<json> responses = this->translator->executeCommand("next", 's');
    for(const auto& response: responses){
        handleMovementResponse(response);
    }
}

static std::string getVariableName(std::string response){
    size_t pos = response.find(' ');
    if(pos == std::string::npos){
        return response;
    } else {
        return response.substr(0, pos);
    }
}

bool GraphDebugger::isVariableInLocals(std::string variableName){
    std::vector<json> responses = this->translator->executeCommand("info locals", 'h');

    for (const auto &response: responses) {
        if(response["type"] == "console") {
            if(getVariableName(response["payload"]) == variableName) {
                return true;
            }
        }
    }

    return false;
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

std::string GraphDebugger::getElementOfArray(const std::string& variableName, int index) {
    std::vector<json> responses = this->translator->
            executeCommand("-data-evaluate-expression *(" + variableName + "+" + to_string(index) + ")", 'h');
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

    std::string addressOfPointerToAdjacencyMatrix = getAddressOfVariable(this->graphVariableName);
    std::vector<std::string> pointersToAdjacencyMatrixLines =
            getValuesByAddress(getValueByAddress(addressOfPointerToAdjacencyMatrix, 0, 8), numberOfVertices);

    int i = 0, j = 0;
    for(const auto& pointerToLine: pointersToAdjacencyMatrixLines){
        std::vector<std::string> valuesOfLine = getValuesByAddress(pointerToLine, numberOfVertices, 4);
        for(const auto& value: valuesOfLine){
            adjacencyMatrix[i][j] = ((int)strtol(value.c_str(), NULL, 16));
            j++;
        }
        i++; j = 0;
    }


    for(const auto& variableName: this->vertexLoads){
        vector<std::string> values(this->numberOfVertices, "");
        for(i = 0; i < this->numberOfVertices; i++){
            values[i] = getElementOfArray(variableName, i);
        }

        loads[variableName] = values;
    }

    return Graph(adjacencyMatrix, loads);
}

void GraphDebugger::setGraph(std::string graph, int n) {
    this->graphVariableName = graph;
    this->numberOfVertices = n;

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
    this->vertexLoads.insert(variableName);

    sendResponse(responseUtils::createSetGraphResponse(true, getGraph()));
}

void GraphDebugger::detachFromVertices(std::string variableName) {
    this->vertexLoads.erase(variableName);

    sendResponse(responseUtils::createSetGraphResponse(true, getGraph()));
}

void GraphDebugger::setWatchOnVertex(int vertexIndex) {
    for(const auto& load: this->vertexLoads) {
        std::vector<json> responses = this->translator->executeCommand("-break-watch "+load + "[" + to_string(vertexIndex)+"]", 'h');
    }

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





