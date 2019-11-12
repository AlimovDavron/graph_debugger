//
// Created by Maruf Karaev on 10/20/19.
//

#ifndef CORE_ARGUMENTPARSER_H
#define CORE_ARGUMENTPARSER_H

#include <iostream>
#include <stdexcept>
#include <map>
#include <string>


class ArgumentParser {
private:
    class Argument {
    public:
        Argument(std::string name, std::string value) : name(name), value(value) {}

        std::string getName() {
            return name;
        }

        std::string getValue() {
            return value;
        }

    private:
        std::string name;
        std::string value;
    };
    static Argument parse_arg(char *arg);

public:
    static std::map<std::string, std::string> parse_args(int argc, char **argv);
};


#endif //CORE_ARGUMENTPARSER_H
