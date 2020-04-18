//
// Created by alimovdavron on 11/12/19.
//

#include "Graph_Debugger.h"

void GraphDebugger::dump() {

}

void GraphDebugger::setTarget(std::string target) {
    cout << this->translator->runCommand("file " + target + "\n") << endl;
}

void GraphDebugger::run() {

}

