#pragma once

#include <memory>
#include <vector>
#include <any>

#include "autogen/Expr.h"

namespace lox {

using std::any;
using std::vector;
using std::unique_ptr;

class Stmt;
class Block;
class Expression;
class If;
class Print;
class VarStmt;

class StmtVisitor
{
public:
    virtual ~StmtVisitor() = default;

    virtual any visitBlockStmt(Block* stmt) = 0;
    virtual any visitExpressionStmt(Expression* stmt) = 0;
    virtual any visitIfStmt(If* stmt) = 0;
    virtual any visitPrintStmt(Print* stmt) = 0;
    virtual any visitVarStmtStmt(VarStmt* stmt) = 0;
};

class Stmt
{
public:
    virtual ~Stmt() = default;

    virtual any accept(StmtVisitor* visitor) = 0;
};

class Block: public Stmt
{
public:
    Block(std::unique_ptr<vector<unique_ptr<Stmt>>> statements): Stmt(), statements(std::move(statements)) {}
    ~Block() override = default;

    any accept(StmtVisitor* visitor) override
    { return visitor->visitBlockStmt(this); }

    std::unique_ptr<vector<unique_ptr<Stmt>>> statements;
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

class If: public Stmt
{
public:
    If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch): Stmt(), condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
    ~If() override = default;

    any accept(StmtVisitor* visitor) override
    { return visitor->visitIfStmt(this); }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
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

class VarStmt: public Stmt
{
public:
    VarStmt(std::unique_ptr<Token> name, std::unique_ptr<Expr> initializer): Stmt(), name(std::move(name)), initializer(std::move(initializer)) {}
    ~VarStmt() override = default;

    any accept(StmtVisitor* visitor) override
    { return visitor->visitVarStmtStmt(this); }

    std::unique_ptr<Token> name;
    std::unique_ptr<Expr> initializer;
};

}