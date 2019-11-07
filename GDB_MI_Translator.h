//
// Created by alimovdavron on 10/29/19.
//

#ifndef CORE_GDB_MI_TRANSLATOR_H
#define CORE_GDB_MI_TRANSLATOR_H

#include "Abstract_GDB_MI_Translator.h"
#include "Abstract_GDB_MI_Output_Parser.h"
#include "PyGDBMIParser.h"

class GDB_MI_Translator : AbstractGDBMITranslator {
private:
    const AbstractGDBMIOutputParser* outputParser;
public:
    explicit GDB_MI_Translator():AbstractGDBMITranslator(){}
    void setBreakpoint(int) override;
    void setBreakpoint(std::string) override;
    void getNBytesAt(unsigned int) override;
    void getValueOf(std::string) override;
    void setWatch(std::string) override;
    void run() override;
    void next() override;
};


#endif //CORE_GDB_MI_TRANSLATOR_H
