//
// Created by alimovdavron on 4/20/20.
//

#ifndef CORE_GRAPHDEBUGGEREXCEPTIONS_H
#define CORE_GRAPHDEBUGGEREXCEPTIONS_H

#include <exception>
#include <iostream>
#include <string>

class ExitException : public std::exception
{
    std::string _msg;
public:
    ExitException(const std::string& msg) : _msg(msg){}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

class ValidationError : public std::exception
{
    std::string _msg;
public:
    ValidationError(const std::string& msg) : _msg(msg){}

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }
};

#endif //CORE_GRAPHDEBUGGEREXCEPTIONS_H
