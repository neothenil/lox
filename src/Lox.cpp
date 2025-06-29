#include "Lox.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <fmt/format.h>

namespace lox {

bool hadError = false;
bool hadRuntimeError = false;
std::unique_ptr<Interpreter> interpreter;

void run(const std::string& source)
{
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();
    Parser parser(std::move(tokens));
    auto statements = parser.parse();

    // Stop if there was a syntax error.
    if (hadError) return;

    if (interpreter.get() == nullptr) {
        interpreter = std::make_unique<Interpreter>();
    }
    interpreter->interpret(statements);
}

void runFile(const std::string& path)
{
    std::ifstream input(path);
    std::stringstream buffer;
    buffer << input.rdbuf();
    run(buffer.str());
    if (hadError) std::exit(65);
    if (hadRuntimeError) std::exit(70);
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

void error(const Token& token, const std::string& message)
{
    if (token.type == TokenType::TOKEN_EOF) {
        report(token.line, " at end", message);
    }
    else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void runtimeError(const RuntimeError& error)
{
    fmt::println("{}\n[line {}]", error.what(), error.token.line);
    hadRuntimeError = true;
}

}