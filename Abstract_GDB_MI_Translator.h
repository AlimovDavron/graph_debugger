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

    json runCommand(std::string command){
        fprintf(gdb, "%s\n", command.c_str());
        fflush(gdb);
        return outputParser->parseOutput(readGDBMIResponse());
    }

public:
    explicit AbstractGDBMITranslator(FILE* gdb, int fileDescriptorFIFO) : gdb(gdb), fileDescriptorFIFO(fileDescriptorFIFO){
        //mknod(FIFO_FILE.c_str(), S_IFIFO | 0666, 0);
        //gdb = popen(("gdb --interpreter=mi --silent > "+FIFO_FILE).c_str(), "w");
        //fileDescriptorFIFO = open(FIFO_FILE.c_str(), O_RDONLY);
        //std::string result = readGDBMIResponse();
    }

    virtual json setTarget(std::string) = 0;
    virtual json setBreakpoint(int) = 0;
    virtual json setBreakpoint(std::string) = 0;
    virtual json getNBytesAt(unsigned int) = 0;
    virtual json getValueOf(std::string) = 0;
    virtual json setWatch(std::string) = 0;
    virtual json run() = 0;
    virtual json next() = 0;
    virtual json gdb_continue() = 0;

};

#endif //CORE_ABSTRACT_GDB_MI_TRANSLATOR_H
