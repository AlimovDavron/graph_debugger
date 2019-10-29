//
// Created by alimovdavron on 10/29/19.
//

#ifndef CORE_GDB_MI_TRANSLATOR_H
#define CORE_GDB_MI_TRANSLATOR_H

#include "Abstract_GDB_MI_Translator.h"

class GDB_MI_Translator : AbstractGDBMITranslator {
public:
    void setBreakpoint(int) override;
    void getNBytesAt(unsigned int) override;
    void getValueOf(std::string) override;
    void setWatch(int) override;
    void run() override;
    void next() override;
};


#endif //CORE_GDB_MI_TRANSLATOR_H
