//
// Created by alimovdavron on 4/27/20.
//

#ifndef CORE_STRUCTURES_H
#define CORE_STRUCTURES_H

struct Position{
    int line;
    std::string file;
    std::string function;

    Position(int line, std::string file, std::string function): line(line), file(file), function(function){}
};

#endif //CORE_STRUCTURES_H
