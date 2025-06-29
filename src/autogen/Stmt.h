#pragma once

#include <memory>
#include <vector>
#include <any>

#include "autogen/Expr.h"

namespace lox {

using any = std::any;

class Stmt;
class Expression;
class Print;

class StmtVisitor
{
public:
    virtual ~StmtVisitor() = default;

    virtual any visitExpressionStmt(Expression* stmt) = 0;
    virtual any visitPrintStmt(Print* stmt) = 0;
};

class Stmt
{
public:
    virtual ~Stmt() = default;

    virtual any accept(StmtVisitor* visitor) = 0;
};

class Expression: public Stmt
{
public:
    Expression(std::unique_ptr<Expr> expr): Stmt(), expr(std::move(expr)) {}
    ~Expression() override = default;

    any accept(StmtVisitor* visitor) override
    { return visitor->visitExpressionStmt(this); }

    std::unique_ptr<Expr> expr;
};

class Print: public Stmt
{
public:
    Print(std::unique_ptr<Expr> expr): Stmt(), expr(std::move(expr)) {}
    ~Print() override = default;

    any accept(StmtVisitor* visitor) override
    { return visitor->visitPrintStmt(this); }

    std::unique_ptr<Expr> expr;
};

}