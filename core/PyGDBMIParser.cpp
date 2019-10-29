//
// Created by alimovdavron on 10/25/19.
//

#include "PyGDBMIParser.h"

json PyGDBMIParser::parseOutput(std::string gdbmiOutput) {
    py::scoped_interpreter guard{};
    py::module gdbmiparser = py::module::import("pygdbmi.gdbmiparser");

    py::object parsedResponse = py::module::import("json")
            .attr("dumps")(gdbmiparser.attr("parse_response")(gdbmiOutput));

    return json::parse(parsedResponse.cast<std::string>());
}