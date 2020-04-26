//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"
#include "../utils/respsonseUtils.h"
using namespace responseUtils;

void GraphDebugger::dump() {

}

//TODO(important):
// 1. handle exiting without code duplication

static void sendResponse(const json& response){
    cout << response << endl;
}

void GraphDebugger::setTarget(const std::string& target) {
    std::vector<json> messages = this->translator->executeCommand("file " + target, 'h');
    this->targetIsSet = true;
    sendResponse(responseUtils::createBinaryResponse(true, "Target is successfully set"));
}

void GraphDebugger::start() {
    std::vector<json> responses = this->translator->executeCommand("start", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
    }

    sendResponse(responseUtils::createBinaryResponse(true, "Debugging started"));
}

void GraphDebugger::continue_() {
    std::vector<json> responses = this->translator->executeCommand("continue", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
    }

    sendResponse(responseUtils::createBinaryResponse(true, "Process stopped"));
}

void GraphDebugger::next() {
    // TODO: come up with output
    std::vector<json> responses = this->translator->executeCommand("next", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
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

std::string GraphDebugger::getAddressOfVariable(std::string variableName) {
    std::vector<json> responses = this->translator->executeCommand("p &" + variableName, 'h');
    for (const auto &response: responses) {
        if(response["type"] == "console"){
            std::string s = response["payload"];
            std::string delim = " ";

            auto start = 0U;
            auto end = s.find(delim);
            while (end != std::string::npos)
            {
                if(s.substr(start, end - start).substr(0, 2) == "0x") {
                    return s.substr(start, end - start);
                }
                start = end + delim.length();
                end = s.find(delim, start);
            }

            if(s.substr(start, end).substr(0, 2) == "0x") {
                return s.substr(start, s.length() - start);
            }
        }
    }
}


void GraphDebugger::setGraph(std::string graph, int n) {
    if(isVariableInLocals(graph)) {
        this->graphVariableName = graph;
        this->numberOfVertices = n;
        std::vector<std::vector<int>> adjacencyMatrix(n, vector<int>(n));

        std::string addressOfPointerToAdjacencyMatrix = getAddressOfVariable(graph);
        std::vector<std::string> pointersToAdjacencyMatrixLines =
                getValuesByAddress(getValueByAddress(addressOfPointerToAdjacencyMatrix), numberOfVertices);

        int i = 0, j = 0;
        for(const auto& pointerToLine: pointersToAdjacencyMatrixLines){
            std::vector<std::string> valuesOfLine = getValuesByAddress(pointerToLine, numberOfVertices, "w");
            for(const auto& value: valuesOfLine){
                adjacencyMatrix[i][j] = ((int)strtol(value.c_str(), NULL, 16));
                j++;
            }
            i++; j = 0;
        }

        sendResponse(responseUtils::createGraphResponse(true, adjacencyMatrix));
    }
}

// todo: Danger zone! Following code works only on 64 bit architecture
std::vector<std::string> GraphDebugger::getValuesByAddress(std::string address, int n, std::string u) {
    std::vector<json> responses = this->translator->
            executeCommand("x/" + std::to_string(n) + "x" + u + " " + address, 'h');

    int unit = (u == "w" ? 10 : 18);

    std::vector<std::string> result;
    for(const auto& response: responses){
        if(response["type"] == "console"){
            std::string payload = response["payload"];

            auto start = 0U;
            size_t pos = payload.find("\\t", start);
            while(pos != std::string::npos){
                result.push_back(payload.substr(pos+2, unit));
                start = pos + 2;
                pos = payload.find("\\t", start);
            }

        }
    }

    return result;
}

std::string GraphDebugger::getValueByAddress(std::string address, std::string u) {
    std::vector<json> responses = this->translator->
            executeCommand("x/1x" + u + " " + address, 'h');

    for(const auto& response: responses){
        if(response["type"] == "console"){
            std::string payload = response["payload"];
            size_t pos = payload.find("\\t");
            return payload.substr(pos+2, 18);
        }
    }
}

void GraphDebugger::setBkpt(int lineNumber) {
    std::vector<json> responses = this->translator->
            executeCommand("break " + std::to_string(lineNumber), 'h');

    sendResponse(responseUtils::createBinaryResponse(true, "Breakpoint is set"));
}



