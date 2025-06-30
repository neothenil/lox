#pragma once

#include <memory>
#include <vector>
#include <any>

#include "Scanner.h"

namespace lox {

using any = std::any;

class Expr;
class Assign;
class Binary;
class Grouping;
class Literal;
class Unary;
class VarExpr;

class ExprVisitor
{
public:
    virtual ~ExprVisitor() = default;

    virtual any visitAssignExpr(Assign* expr) = 0;
    virtual any visitBinaryExpr(Binary* expr) = 0;
    virtual any visitGroupingExpr(Grouping* expr) = 0;
    virtual any visitLiteralExpr(Literal* expr) = 0;
    virtual any visitUnaryExpr(Unary* expr) = 0;
    virtual any visitVarExprExpr(VarExpr* expr) = 0;
};

class Expr
{
public:
    virtual ~Expr() = default;

    virtual any accept(ExprVisitor* visitor) = 0;
};

class Assign: public Expr
{
public:
    Assign(std::unique_ptr<Token> name, std::unique_ptr<Expr> value): Expr(), name(std::move(name)), value(std::move(value)) {}
    ~Assign() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitAssignExpr(this); }

    std::unique_ptr<Token> name;
    std::unique_ptr<Expr> value;
};

class Binary: public Expr
{
public:
    Binary(std::unique_ptr<Expr> left, std::unique_ptr<Token> op, std::unique_ptr<Expr> right): Expr(), left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
    ~Binary() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitBinaryExpr(this); }

    std::unique_ptr<Expr> left;
    std::unique_ptr<Token> op;
    std::unique_ptr<Expr> right;
};

class Grouping: public Expr
{
public:
    Grouping(std::unique_ptr<Expr> expression): Expr(), expression(std::move(expression)) {}
    ~Grouping() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitGroupingExpr(this); }

    std::unique_ptr<Expr> expression;
};

class Literal: public Expr
{
public:
    Literal(std::unique_ptr<Token> value): Expr(), value(std::move(value)) {}
    ~Literal() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitLiteralExpr(this); }

    std::unique_ptr<Token> value;
};

class Unary: public Expr
{
public:
    Unary(std::unique_ptr<Token> op, std::unique_ptr<Expr> right): Expr(), op(std::move(op)), right(std::move(right)) {}
    ~Unary() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitUnaryExpr(this); }

    std::unique_ptr<Token> op;
    std::unique_ptr<Expr> right;
};

class VarExpr: public Expr
{
public:
    VarExpr(std::unique_ptr<Token> name): Expr(), name(std::move(name)) {}
    ~VarExpr() override = default;

    any accept(ExprVisitor* visitor) override
    { return visitor->visitVarExprExpr(this); }

    std::unique_ptr<Token> name;
};

}