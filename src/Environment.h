#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include "Scanner.h"

namespace lox {

class Environment
{
public:
    Environment() = default;

    void define(const std::string& name, const std::any& value);
    std::any& get(const Token& name);
    void assign(const Token& name, const std::any& value);

private:
    std::unordered_map<std::string, std::any> values;
};

}