#include "Environment.h"
#include "Interpreter.h"
#include <fmt/format.h>

namespace lox {

void Environment::define(const std::string& name, const std::any& value)
{
    values.insert({name, value});
}

std::any& Environment::get(const Token& name)
{
    if (values.find(name.lexeme) != values.end()) {
        return values.at(name.lexeme);
    }

    throw RuntimeError(name,
        fmt::format("Undefined variable '{}'.", name.lexeme));
}

void Environment::assign(const Token& name, const std::any& value)
{
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    throw RuntimeError(name,
        fmt::format("Undefined variable '{}'.", name.lexeme));
}

}