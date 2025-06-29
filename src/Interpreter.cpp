#include "Interpreter.h"
#include "Lox.h"
#include <fmt/format.h>
#include <cmath>

namespace lox {

any Interpreter::evaluate(Expr* expr)
{
    return expr->accept(this);
}

void Interpreter::execute(Stmt* stmt)
{
    stmt->accept(this);
}

bool Interpreter::isTruthy(const any* obj)
{
    if (obj == nullptr) return false;
    if (!obj->has_value()) return false;
    if (auto ptr = std::any_cast<nullptr_t>(obj)) {
        return false;
    }
    if (auto ptr = std::any_cast<bool>(obj)) {
        return *ptr;
    }
    return true;
}

bool Interpreter::isEqual(const any& a, const any& b)
{
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value() || !b.has_value()) return false;
    if (a.type() != b.type()) return false;
    if (std::any_cast<double>(&a)) {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    if (std::any_cast<std::string>(&a)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    if (std::any_cast<bool>(&a)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }
    if (std::any_cast<nullptr_t>(&a)) {
        return std::any_cast<nullptr_t>(a) == std::any_cast<nullptr_t>(b);
    }
    // unreachable
    return false;
}

void Interpreter::checkNumberOperand(const Token& op, const any& operand)
{
    if (std::any_cast<double>(&operand)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op,
    const any& left, const any& right)
{
    if (std::any_cast<double>(&left) && std::any_cast<double>(&right)) {
        return;
    }
    throw RuntimeError(op, "Operands must be numbers.");
}

std::string Interpreter::stringify(const any& obj)
{
    if (!obj.has_value()) return std::string();
    if (auto ptr = std::any_cast<nullptr_t>(&obj)) {
        return "nil";
    }
    if (auto ptr = std::any_cast<double>(&obj)) {
        double integral = 0.0;
        if (std::modf(*ptr, &integral) == 0.0) {
            return fmt::format("{:.0f}", *ptr);
        }
        return fmt::format("{}", *ptr);
    }
    if (auto ptr = std::any_cast<bool>(&obj)) {
        if (*ptr) return "true";
        else return "false";
    }
    if (auto ptr = std::any_cast<std::string>(&obj)) {
        return *ptr;
    }
    return std::string();
}

any Interpreter::visitBinaryExpr(Binary* expr)
{
    auto left = evaluate(expr->left.get());
    auto right = evaluate(expr->right.get());

    switch (expr->op->type) {
    case TokenType::GREATER:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
    case TokenType::GREATER_EQUAL:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case TokenType::LESS:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
    case TokenType::LESS_EQUAL:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
    case TokenType::MINUS:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TokenType::PLUS: {
        if (std::any_cast<double>(&left) && std::any_cast<double>(&right)) {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }
        if (std::any_cast<std::string>(&left) &&
            std::any_cast<std::string>(&right)) {
            return std::any_cast<std::string>(left)
                + std::any_cast<std::string>(right);
        }
        throw RuntimeError(*expr->op.get(),
            "Operands must be two numbers or two strings.");
    }
    case TokenType::SLASH:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);
    case TokenType::STAR:
        checkNumberOperands(*expr->op.get(), left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    case TokenType::BANG_EQUAL:
        return !isEqual(left, right);
    case TokenType::EQUAL_EQUAL:
        return isEqual(left, right);
    default:
        break;
    }

    return any();
}

any Interpreter::visitGroupingExpr(Grouping* expr)
{
    return evaluate(expr->expression.get());
}

any Interpreter::visitLiteralExpr(Literal* expr)
{
    return expr->value->literal;
}

any Interpreter::visitUnaryExpr(Unary* expr)
{
    auto right = evaluate(expr->right.get());

    switch (expr->op->type) {
    case TokenType::MINUS:
        checkNumberOperand(*expr->op.get(), right);
        return -std::any_cast<double>(right);
    case TokenType::BANG:
        return !isTruthy(&right);
    default:
        break;
    }

    return any();
}

any Interpreter::visitExpressionStmt(Expression* stmt)
{
    evaluate(stmt->expr.get());
    return any();
}

any Interpreter::visitPrintStmt(Print* stmt)
{
    auto value = evaluate(stmt->expr.get());
    fmt::println(stringify(value));
    return any();
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& statements)
{
    try {
        for (auto& statement : statements) {
            execute(statement.get());
        }
    }
    catch (const RuntimeError& error) {
        runtimeError(error);
    }
}

}