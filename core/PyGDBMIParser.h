//
// Created by alimovdavron on 10/25/19.
//

#ifndef CORE_PYGDBMIPARSER_H
#define CORE_PYGDBMIPARSER_H

#include "Abstract_GDB_MI_Output_Parser.h"
#include "json.hpp"
#include <pybind11/embed.h>
#include <string>

namespace py = pybind11;
using json = nlohmann::json;

class PyGDBMIParser : AbstractGDBMIOutputParser {
public:
    json parseOutput(std::string GDBMIText) override;
};


#endif //CORE_PYGDBMIPARSER_H
