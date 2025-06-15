#pragma once

#include "autogen/Expr.h"
#include <initializer_list>

namespace lox {

class AstPrinter: public ExprVisitor
{
public:
    ~AstPrinter() override = default;
    any visitBinaryExpr(Binary* expr) override;
    any visitGroupingExpr(Grouping* expr) override;
    any visitLiteralExpr(Literal* expr) override;
    any visitUnaryExpr(Unary* expr) override;

    std::string print(Expr* expr) {
        return std::any_cast<std::string>(expr->accept(this));
    }

private:
    std::string anyToString(const any* obj);
    std::string parenthesize(const std::string& name,
        std::initializer_list<Expr*> exprs);
};

void printAst(Expr* expression);

}
