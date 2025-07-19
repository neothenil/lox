#pragma once

#include "Interpreter.h"

namespace lox {

class LoxCallable
{
public:
    virtual ~LoxCallable() = default;
    virtual int arity() = 0;
    virtual any call(Interpreter* interpreter, std::vector<any>& arguments) = 0;
};

}