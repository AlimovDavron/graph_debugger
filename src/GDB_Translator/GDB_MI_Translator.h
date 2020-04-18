//
// Created by alimovdavron on 10/29/19.
//

#ifndef CORE_GDB_MI_TRANSLATOR_H
#define CORE_GDB_MI_TRANSLATOR_H

#include "Abstract_GDB_MI_Output_Parser.h"
#include "PyGDBMIParser.h"

using namespace std;
#include "iostream"

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

public:
    GDB_MI_Translator(FILE* gdb, int fileDescriptorFIFO):gdb(gdb), fileDescriptorFIFO(fileDescriptorFIFO) {
        flushGDBMIResponse();
        outputParser = (AbstractGDBMIOutputParser*)new PyGDBMIParser();
    }

    std::string flushGDBMIResponse();

    std::vector<json> readGDBMIResponse(std::string hash);

    static std::vector<std::string> split(std::string a, std::string delimiter);

    json runCommand(std::string command);
};


#endif //CORE_GDB_MI_TRANSLATOR_H
