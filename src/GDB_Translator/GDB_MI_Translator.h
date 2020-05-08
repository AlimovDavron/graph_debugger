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

    std::string flushGDBMIResponse();

    std::vector<json> readGDMIResponseUntilHash(int hash);
    std::vector<json> readGDMIResponseUntilStop();

    static std::vector<std::string> split(std::string a, const std::string& delimiter);

public:
    GDB_MI_Translator(FILE* gdb, int fileDescriptorFIFO):gdb(gdb), fileDescriptorFIFO(fileDescriptorFIFO) {
        flushGDBMIResponse();
        executeCommandNoJSON("set can-use-hw-watchpoints 0");
        flushGDBMIResponse();
        outputParser = (AbstractGDBMIOutputParser*)new PyGDBMIParser();
    }

    std::vector<json> executeCommand(std::string command, char mode);
    void executeCommandNoJSON(std::string command);
};


#endif //CORE_GDB_MI_TRANSLATOR_H
