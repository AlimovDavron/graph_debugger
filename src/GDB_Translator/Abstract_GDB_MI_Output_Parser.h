//
// Created by alimovdavron on 10/26/19.
//

#ifndef CORE_ABSTRACT_GDB_MI_OUTPUT_PARSER_H
#define CORE_ABSTRACT_GDB_MI_OUTPUT_PARSER_H

#include <string>
#include "../../json.hpp"
using json = nlohmann::json;

class AbstractGDBMIOutputParser {
public:
    virtual json parseOutput(std::string) = 0;
};

#endif //CORE_ABSTRACT_GDB_MI_OUTPUT_PARSER_H
