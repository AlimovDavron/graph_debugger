//
// Created by alimovdavron on 10/29/19.
//

#ifndef CORE_GDB_MI_TRANSLATOR_H
#define CORE_GDB_MI_TRANSLATOR_H

#include "Abstract_GDB_MI_Translator.h"
#include "Abstract_GDB_MI_Output_Parser.h"
#include "PyGDBMIParser.h"

class GDB_MI_Translator : AbstractGDBMITranslator {
public:
    explicit GDB_MI_Translator(FILE* gdb, int fileDescriptorFIFO):AbstractGDBMITranslator(gdb, fileDescriptorFIFO){
        readGDBMIResponse();
        outputParser = (AbstractGDBMIOutputParser*)new PyGDBMIParser();
    }
    json setTarget(std::string) override;
    json setBreakpoint(int) override;
    json setBreakpoint(std::string) override;
    json getNBytesAt(unsigned int) override;
    json getValueOf(std::string) override;
    json setWatch(std::string) override;
    json run() override;
    json next() override;
    json help(){
        return runCommand("help");
    }
};


#endif //CORE_GDB_MI_TRANSLATOR_H
