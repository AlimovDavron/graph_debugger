//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"

void GraphDebugger::dump() {

}

//TODO(important):
// 1. handle exiting without code duplication

void GraphDebugger::setTarget(const std::string& target) {
    std::vector<json> messages = this->translator->executeCommand("file " + target, 'h');
    // TODO: come up with output
}

void GraphDebugger::start() {
    std::vector<json> responses = this->translator->executeCommand("start", 's');
    // TODO: come up with output
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
    }
}

void GraphDebugger::continue_() {
    // TODO: come up with output
    std::vector<json> responses = this->translator->executeCommand("continue", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
    }
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

void GraphDebugger::setGraph(std::string graph) {
    if(isVariableInLocals(graph)) {

        std::string address = getAddressOfVariable(graph);
        cout << "ADDRESS:" << address << endl;
        /*std::vector<json> responses = this->translator->executeCommand("watch " + graph, 'h');
        for (const auto &response: responses) {
            if (response["type"] == "notify" && response["message"] == "stopped") {
                if (response["payload"]["reason"] == "exited-normally")
                    throw ExitException("success");
            }
            cout << response << endl;
        }*/
    }
}



