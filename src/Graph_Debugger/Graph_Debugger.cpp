//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"

void GraphDebugger::dump() {

}

void GraphDebugger::setTarget(const std::string& target) {
    std::vector<json> messages = this->translator->executeCommand("file " + target, 'h');
    for(const auto& u: messages){
        cout << u << endl;
    }
}

void GraphDebugger::start() {
    std::vector<json> responses = this->translator->executeCommand("start", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
        cout << response << endl;
    }
}

void GraphDebugger::continue_() {
    std::vector<json> responses = this->translator->executeCommand("continue", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
        cout << response << endl;
    }
}

void GraphDebugger::next() {
    std::vector<json> responses = this->translator->executeCommand("next", 's');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
        cout << response << endl;
    }
}

void GraphDebugger::setGraph(const std::string& graph) {
    std::vector<json> responses = this->translator->executeCommand("watch " + graph, 'h');
    for(const auto& response: responses){
        if(response["type"] == "notify" && response["message"] == "stopped") {
            if(response["payload"]["reason"] == "exited-normally")
                throw ExitException("success");
        }
        cout << response << endl;
    }
}

