//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"

void GraphDebugger::dump() {

}

void GraphDebugger::setTarget(std::string target) {
    std::vector<json> messages = this->translator->executeCommand("file " + target + "\n", 'h');
    for(const auto& u: messages){
        cout << u << endl;
    }
}

void GraphDebugger::run() {

}

