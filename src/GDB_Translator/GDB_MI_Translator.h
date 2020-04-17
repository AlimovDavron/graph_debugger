//
// Created by alimovdavron on 10/29/19.
//

#ifndef CORE_GDB_MI_TRANSLATOR_H
#define CORE_GDB_MI_TRANSLATOR_H

#include "Abstract_GDB_MI_Translator.h"
#include "Abstract_GDB_MI_Output_Parser.h"
#include "PyGDBMIParser.h"

class GDB_MI_Translator {
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
        // TODO: return vector of actions
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
        // TODO: generate hash
        fprintf(gdb, "%s\n", command.c_str());
        fflush(gdb);
        return outputParser->parseOutput(readGDBMIResponse());
    }

public:
    GDB_MI_Translator(FILE* gdb, int fileDescriptorFIFO):gdb(gdb), fileDescriptorFIFO(fileDescriptorFIFO) {
        readGDBMIResponse();
        outputParser = (AbstractGDBMIOutputParser*)new PyGDBMIParser();
    }
};


#endif //CORE_GDB_MI_TRANSLATOR_H
