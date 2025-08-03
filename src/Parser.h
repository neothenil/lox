#pragma once

#include "Scanner.h"
#include "autogen/Expr.h"
#include "autogen/Stmt.h"
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

    std::vector<std::unique_ptr<Stmt>> parse();

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

    /* parsing functions for grammar:
     * program        → declaration* EOF ;
     * declaration    → funDecl
     *                | varDecl
     *                | statement ;
     * funDecl        → "fun" function ;
     * function       → IDENTIFIER "(" parameters? ")" block ;
     * parameters     → IDENTIFIER ( "," IDENTIFIER )* ;
     * varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
     * statement      → exprStmt
     *                | forStmt
     *                | ifStmt
     *                | printStmt
     *                | returnStmt
     *                | whileStmt
     *                | block;
     * exprStmt       → expression ";" ;
     * forStmt        → "for" "(" ( varDecl | exprStmt | ";" )
     *                  expression? ";"
     *                  expression? ")" statement ;
     * ifStmt         → "if" "(" expression ")" statement
     *                ( "else" statement )? ;
     * printStmt      → "print" expression ";" ;
     * returnStmt     → "return" expression? ";" ;
     * whileStmt      → "while" "(" expression ")" statement ;
     * block          → "{" declaration* "}" ;
     * expression     → assignment ;
     * assignment     → IDENTIFIER "=" assignment
     *                | logical_or ;
     * logic_or       → logic_and ( "or" logic_and )* ;
     * logic_and      → equality ( "and" equality )* ;
     * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
     * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
     * term           → factor ( ( "-" | "+" ) factor )* ;
     * factor         → unary ( ( "/" | "*" ) unary )* ;
     * unary          → ( "!" | "-" ) unary
     *                | call ;
     * call           → primary ( "(" arguments? ")" )* ;
     * arguments      → expression ( "," expression )* ;
     * primary        → NUMBER | STRING | "true" | "false" | "nil"
     *                | "(" expression ")" | IDENTIFIER;
     * */
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<vector<unique_ptr<Stmt>>> block();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> logicOr();
    std::unique_ptr<Expr> logicAnd();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> primary();

    std::unique_ptr<Function> function(const std::string& kind);
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);

    std::vector<Token> tokens;
    int current = 0;
};

}