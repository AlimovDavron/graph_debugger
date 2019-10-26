//
// Created by alimovdavron on 10/26/19.
//

#ifndef CORE_ABSTRACT_GDB_MI_TRANSLATOR_H
#define CORE_ABSTRACT_GDB_MI_TRANSLATOR_H

#include <cstdio>
#include <string>

class AbstractGDBMITranslator {
private:
    FILE* pipe;
public:
    explicit AbstractGDBMITranslator(FILE* pipe):pipe(pipe){}
    void setTarget(const char* path) {
        fprintf(pipe, "file %s\n", path);
    }

    virtual void setBreakpoint(int) = 0;
    virtual void getNBytesAt(unsigned int) = 0;
    virtual void getValueOf(std::string) = 0;
    virtual void setWatch(int) = 0;
    virtual void run() = 0;
    virtual void next() = 0;

};

#endif //CORE_ABSTRACT_GDB_MI_TRANSLATOR_H
