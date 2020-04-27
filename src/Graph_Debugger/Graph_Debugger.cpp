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

void GraphDebugger::handleMovementResponse(const json & response) {
    if(response["type"] == "notify" && response["message"] == "stopped") {

        if(response["payload"]["reason"] == "signal-received"){
            sendResponse(responseUtils::createErrorStopResponse(
                    response["payload"]["reason"],
                    response["payload"]["signal-meaning"],
                    getAdjacencyMatrix(),
                    getCurrentPosition()));
            return;
        }

        if(response["payload"]["reason"] == "exited-normally" or
           response["payload"]["reason"] == "exited-signalled")
            throw ExitException("success");


        sendResponse(responseUtils::createStopResponse(
                response["payload"]["reason"],
                getAdjacencyMatrix(),
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

std::vector<std::vector<int>> GraphDebugger::getAdjacencyMatrix() {
    if(numberOfVertices == -1){
        return std::vector<std::vector<int>>();
    }

    std::vector<std::vector<int>> adjacencyMatrix(this->numberOfVertices, vector<int>(this->numberOfVertices));

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

    return adjacencyMatrix;
}

void GraphDebugger::setGraph(std::string graph, int n) {
    this->graphVariableName = graph;
    this->numberOfVertices = n;

    sendResponse(responseUtils::createSetGraphResponse(true, getAdjacencyMatrix()));
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
            executeCommand("break " + std::to_string(lineNumber), 'h');

    for(const auto& response: responses){
        if(response["type"] == "notify"){
            sendResponse(responseUtils::createBinaryResponse(true, response["message"]));
            return;
        }
    }

    sendResponse(responseUtils::createBinaryResponse(false, "Error"));
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

void GraphDebugger::debug() {
    std::vector<json> responses = this->translator->
            executeCommand("-stack-info-frame", 'h');

    for(const auto& response: responses){
        cout << response << endl;
    }
}







