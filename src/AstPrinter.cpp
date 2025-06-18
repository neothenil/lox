#include "AstPrinter.h"
#include <sstream>
#include <fmt/format.h>

namespace lox {

any AstPrinter::visitBinaryExpr(Binary* expr)
{
    return parenthesize(expr->op->lexeme, {
        expr->left.get(), expr->right.get()
    });
}

any AstPrinter::visitGroupingExpr(Grouping* expr)
{
    return parenthesize("group", {expr->expression.get()});
}

std::string AstPrinter::anyToString(const any* obj)
{
    if (const std::string* ptr = std::any_cast<std::string>(obj)) {
        return *ptr;
    }
    if (const double* ptr = std::any_cast<double>(obj)) {
        return std::to_string(*ptr);
    }
    if (const int32_t* ptr = std::any_cast<int32_t>(obj)) {
        return std::to_string(*ptr);
    }
    if (const bool* ptr = std::any_cast<bool>(obj)) {
        if (*ptr) return "true";
        else return "false";
    }
    if (const nullptr_t* ptr = std::any_cast<nullptr_t>(obj)) {
        return "nil";
    }
    return std::string();
}

any AstPrinter::visitLiteralExpr(Literal* expr)
{
    return anyToString(expr->value.get());
}

any AstPrinter::visitUnaryExpr(Unary* expr)
{
    return parenthesize(expr->op->lexeme, {expr->right.get()});
}

std::string AstPrinter::parenthesize(const std::string& name,
    std::initializer_list<Expr*> exprs)
{
    std::stringstream ss;
    ss << "(" << name;
    for (Expr* expr: exprs) {
        ss << " ";
        ss << std::any_cast<std::string>(expr->accept(this));
    }
    ss << ")";
    return ss.str();
}

void printAst(Expr* expression)
{
    // -123
    auto minus = std::make_unique<lox::Token>(lox::TokenType::MINUS, "-", std::any(), 1);
    auto i123 = std::make_unique<lox::Literal>(std::make_unique<std::any>(int32_t(123)));
    auto left = std::make_unique<lox::Unary>(std::move(minus), std::move(i123));

    // (45.67)
    auto right = std::make_unique<lox::Grouping>(
        std::make_unique<lox::Literal>(std::make_unique<std::any>(45.67)));

    // *
    auto star = std::make_unique<lox::Token>(lox::TokenType::STAR, "*", std::any(), 1);
    auto expr = std::make_unique<lox::Binary>(std::move(left), std::move(star), std::move(right));
    lox::AstPrinter printer;
    fmt::println(printer.print(expr.get()));
}

}
