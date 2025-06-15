#include "Lox.h"
#include "AstPrinter.h"
#include <iostream>
#include <fmt/format.h>

// int main(int argc, const char* argv[])
// {
//     if (argc > 2) {
//         std::cout << "Usage: lox [script]" << std::endl;
//         std::exit(64);
//     }
//     else if (argc == 2) {
//         lox::runFile(argv[1]);
//     }
//     else {
//         lox::runPrompt();
//     }
//     return 0;
// }

int main(int argc, const char* argv[])
{
    lox::printAst(nullptr);
    return 0;
}