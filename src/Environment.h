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
    Environment(std::shared_ptr<Environment> enclosing):
        enclosing(enclosing) {}

    void define(const std::string& name, const std::any& value);
    std::any& get(const Token& name);
    void assign(const Token& name, const std::any& value);
    std::shared_ptr<Environment> pop();

private:
    std::unordered_map<std::string, std::any> values;
    // memory leakage will occur.
    std::shared_ptr<Environment> enclosing;
};

class EnvironmentGuard
{
public:
    EnvironmentGuard(std::shared_ptr<Environment>& env): env(env) {}
    ~EnvironmentGuard() { env = env->pop(); }

private:
    std::shared_ptr<Environment>& env;
};

class EnvironmentSwapGuard
{
public:
    EnvironmentSwapGuard(std::shared_ptr<Environment>& cur,
        std::shared_ptr<Environment>& pre): cur(cur), pre(pre) {}
    ~EnvironmentSwapGuard() { std::swap(cur, pre); }

private:
    std::shared_ptr<Environment>& cur;
    std::shared_ptr<Environment>& pre;
};

}