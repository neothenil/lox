#include "Lox.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cout << "Usage: lox [script]" << std::endl;
        std::exit(64);
    }
    else if (argc == 2) {
        lox::runFile(argv[1]);
    }
    else {
        lox::runPrompt();
    }
    return 0;
}