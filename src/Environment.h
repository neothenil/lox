#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include <memory>
#include "Scanner.h"

namespace lox {

class Environment
{
public:
    Environment() = default;
    Environment(std::unique_ptr<Environment> enclosing):
        enclosing(std::move(enclosing)) {}

    void define(const std::string& name, const std::any& value);
    std::any& get(const Token& name);
    void assign(const Token& name, const std::any& value);
    std::unique_ptr<Environment> pop();

private:
    std::unordered_map<std::string, std::any> values;
    std::unique_ptr<Environment> enclosing;
};

class EnvironmentGuard
{
public:
    EnvironmentGuard(std::unique_ptr<Environment>& env): env(env) {}
    ~EnvironmentGuard() { env = env->pop(); }

private:
    std::unique_ptr<Environment>& env;
};

}