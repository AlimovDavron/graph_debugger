//
// Created by alimovdavron on 10/26/19.
//

#ifndef CORE_ABSTRACT_GDB_MI_COMMUNICATOR_H
#define CORE_ABSTRACT_GDB_MI_COMMUNICATOR_H

#include <cstdio>

class AbstractGDBMICommunicator {
private:
    FILE* pipe;
public:
    explicit AbstractGDBMICommunicator(FILE* pipe):pipe(pipe){}
    void setTarget(const char* path) {
        fprintf(pipe, "file %s\n", path);
    }

    virtual void setBreakpoint(int) = 0;
    virtual void getGraphState() = 0;
    virtual void setWatch(int) = 0;
    virtual void runAndCatch() = 0;

};

#endif //CORE_ABSTRACT_GDB_MI_COMMUNICATOR_H
