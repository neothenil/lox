#pragma once

#include "autogen/Expr.h"
#include <stdexcept>

namespace lox {

class RuntimeError: public std::runtime_error
{
public:
    RuntimeError(const Token& token, const std::string& message):
        std::runtime_error(message), token(token) {}

    Token token;
};

class Interpreter: public ExprVisitor
{
public:
    ~Interpreter() override = default;
    any visitBinaryExpr(Binary* expr) override;
    any visitGroupingExpr(Grouping* expr) override;
    any visitLiteralExpr(Literal* expr) override;
    any visitUnaryExpr(Unary* expr) override;

    void interpret(Expr* expr);

private:
    any evaluate(Expr* expr);
    bool isTruthy(const any* obj);
    bool isEqual(const any& a, const any& b);
    void checkNumberOperand(const Token& op, const any& operand);
    void checkNumberOperands(const Token& op,
        const any& left, const any& right);
    std::string stringify(const any& obj);
};

}