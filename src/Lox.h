#pragma once

#include "Scanner.h"
#include <string>

namespace lox {
extern bool hadError;

void runFile(const std::string& path);
void runPrompt();

void error(int line, const std::string& message);
void error(const Token& token, const std::string& message);
}
