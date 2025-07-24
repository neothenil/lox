#pragma once

#include <chrono>
#include "Interpreter.h"

namespace lox {

class LoxCallable
{
public:
    virtual ~LoxCallable() = default;
    virtual int arity() = 0;
    virtual any call(Interpreter* interpreter, std::vector<any>& arguments) = 0;
};

class ClockCallable: public LoxCallable
{
public:
    ~ClockCallable() override = default;
    int arity() override { return 0; }
    any call(Interpreter* interpreter, std::vector<any>& arguments) override
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
            duration).count();
        return (double)millis / 1000.0;
    }
};

}