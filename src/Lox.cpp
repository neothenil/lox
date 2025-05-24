#include "Lox.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <fmt/format.h>

namespace lox {

bool hadError = false;

void run(const std::string& source)
{
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    // For now, just print the tokens.
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }
}

void runFile(const std::string& path)
{
    std::ifstream input(path);
    std::stringstream buffer;
    buffer << input.rdbuf();
    run(buffer.str());
    if (hadError) std::exit(65);
}

void runPrompt()
{
    while (true) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof()) break;
        run(line);
        hadError = false;
    }
}

static void report(int line, const std::string& where,
    const std::string& message)
{
    fmt::println("[line {}] Error{}: {}", line, where, message);
    hadError = true;
}

void error(int line, const std::string& message)
{
    report(line, "", message);
}

}