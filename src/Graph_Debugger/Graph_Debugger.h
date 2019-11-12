//
// Created by alimovdavron on 11/12/19.
//

#ifndef CORE_GRAPH_DEBUGGER_H
#define CORE_GRAPH_DEBUGGER_H

#include <iostream>

class GraphDebugger {
protected:
    int FIFOFileDescriptor;
    FILE *gdb;
    GraphDebugger(){};
    GraphDebugger* instance = nullptr;
public:
    GraphDebugger* getInstance(){
        if(instance == nullptr){
            instance = new GraphDebugger();
        }

        return instance;
    }

    void dump();
};

#endif //CORE_GRAPH_DEBUGGER_H
