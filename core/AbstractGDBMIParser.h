//
// Created by alimovdavron on 10/25/19.
//

#ifndef CORE_ABSTRACTGDBMIPARSER_H
#define CORE_ABSTRACTGDBMIPARSER_H

#include <string>

class AbstractGDBMIParser {
    virtual std::string parseOutput(std::string) = 0;
};

#endif //CORE_ABSTRACTGDBMIPARSER_H
