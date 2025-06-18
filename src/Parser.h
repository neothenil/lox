#pragma once

#include "Scanner.h"
#include "autogen/Expr.h"
#include <stdexcept>

namespace lox {

class ParserError: public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
    ParserError(): ParserError(std::string()) {}
};

class Parser
{
public:
    Parser(std::vector<Token>&& tokens): tokens(std::move(tokens)) {}

    std::unique_ptr<Expr> parse();

private:
    // utility functions
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    ParserError error(const Token& token, const std::string& message);
    bool match(std::initializer_list<TokenType> types);
    void synchronize();

    Token consume(TokenType type, const std::string& message);

    /* parse functions for grammar:
     * expression     → equality ;
     * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
     * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
     * term           → factor ( ( "-" | "+" ) factor )* ;
     * factor         → unary ( ( "/" | "*" ) unary )* ;
     * unary          → ( "!" | "-" ) unary
     *                | primary ;
     * primary        → NUMBER | STRING | "true" | "false" | "nil"
     *                | "(" expression ")" ;
     * */
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    std::vector<Token> tokens;
    int current = 0;
};

}