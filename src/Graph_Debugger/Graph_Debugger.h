//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPH_DEBUGGER_H
#define CORE_GRAPH_DEBUGGER_H

#include <iostream>
#include "../GDB_Translator/GDB_MI_Translator.h"

class GraphDebugger {
private:
    GDB_MI_Translator* translator;

public:
    GraphDebugger(int FIFOFileDescriptor, FILE *gdb){
        translator = new GDB_MI_Translator(gdb, FIFOFileDescriptor);
    }

    void dump();
};

#endif //CORE_GRAPH_DEBUGGER_H
