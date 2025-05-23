#include "Lox.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace lox {

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
}

void runPrompt()
{
    while (true) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (std::cin.eof()) break;
        run(line);
    }
}

}