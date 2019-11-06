//
// Created by alimovdavron on 10/26/19.
//

#ifndef CORE_ABSTRACT_GDB_MI_TRANSLATOR_H
#define CORE_ABSTRACT_GDB_MI_TRANSLATOR_H

#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Abstract_GDB_MI_Output_Parser.h"

const std::string FIFO_FILE = "graph_debugger";

class AbstractGDBMITranslator {
protected:
    FILE* gdb;
    int fileDescriptorFIFO;
    AbstractGDBMIOutputParser* outputParser;

    static bool has_suffix(const std::string &str, const std::string &suffix)
    {
        return str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    std::string readGDBMIResponse(){
        int size;
        char buf[4096];
        std::string result;
        while((size = read(fileDescriptorFIFO, buf, sizeof(buf) - 1)) > 0){
            buf[size] = '\0';
            result += buf;
            if(has_suffix(result, "(gdb) \n")){
                return result;
            }
        }
    }

public:
    explicit AbstractGDBMITranslator(){
        gdb = popen(("gdb --interpreter=mi --silent > "+FIFO_FILE).c_str(), "w");
        fileDescriptorFIFO = open(FIFO_FILE.c_str(), O_RDONLY);
        std::string result = readGDBMIResponse();
    }

    void setTarget(const char* path) {
        fprintf(gdb, "file %s\n", path);
    }

    virtual void setBreakpoint(int) = 0;
    virtual void setBreakpoint(std::string) = 0;
    virtual void getNBytesAt(unsigned int) = 0;
    virtual void getValueOf(std::string) = 0;
    virtual void setWatch(std::string) = 0;
    virtual void run() = 0;
    virtual void next() = 0;

    ~AbstractGDBMITranslator(){
        remove(FIFO_FILE.c_str());
        pclose(gdb);
    }

};

#endif //CORE_ABSTRACT_GDB_MI_TRANSLATOR_H
