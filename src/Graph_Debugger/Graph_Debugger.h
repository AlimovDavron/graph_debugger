//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPH_DEBUGGER_H
#define CORE_GRAPH_DEBUGGER_H

#include <iostream>
#include "../GDB_Translator/GDB_MI_Translator.h"
#include "GraphDebuggerExceptions.h"

class GraphDebugger {
private:
    GDB_MI_Translator* translator;
    FILE* gdb;
    bool isVariableInLocals(std::string variableName);
    std::string getAddressOfVariable(std::string variableName);

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
    void setGraph(std::string);
};

#endif //CORE_GRAPH_DEBUGGER_H
