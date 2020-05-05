//
// Created by alimovdavron on 5/4/20.
//

#ifndef CORE_CONSOLEINTERFACEEXCEPTIONS_H
#define CORE_CONSOLEINTERFACEEXCEPTIONS_H

#include <exception>
#include <iostream>
#include <string>

class InputErrorException : public std::exception
{
    std::string _msg;
public:
    InputErrorException(const std::string& msg) : _msg(msg){}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

#endif //CORE_CONSOLEINTERFACEEXCEPTIONS_H
