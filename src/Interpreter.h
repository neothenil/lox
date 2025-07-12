#pragma once

#include "autogen/Expr.h"
#include "autogen/Stmt.h"
#include "Environment.h"
#include <stdexcept>

namespace lox {

class RuntimeError: public std::runtime_error
{
public:
    RuntimeError(const Token& token, const std::string& message):
        std::runtime_error(message), token(token) {}

    Token token;
};

class Interpreter: public ExprVisitor, public StmtVisitor
{
public:
    Interpreter(): environment(std::make_unique<Environment>()) {}
    ~Interpreter() override = default;

    any visitAssignExpr(Assign* expr) override;
    any visitBinaryExpr(Binary* expr) override;
    any visitGroupingExpr(Grouping* expr) override;
    any visitLiteralExpr(Literal* expr) override;
    any visitLogicalExpr(Logical* expr) override;
    any visitUnaryExpr(Unary* expr) override;
    any visitVarExprExpr(VarExpr* expr) override;

    any visitBlockStmt(Block* stmt) override;
    any visitExpressionStmt(Expression* stmt) override;
    any visitIfStmt(If* stmt) override;
    any visitPrintStmt(Print* stmt) override;
    any visitVarStmtStmt(VarStmt* stmt) override;

    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);

private:
    any evaluate(Expr* expr);
    void execute(Stmt* stmt);
    void executeBlock(vector<unique_ptr<Stmt>>* statements);
    bool isTruthy(const any* obj);
    bool isEqual(const any& a, const any& b);
    void checkNumberOperand(const Token& op, const any& operand);
    void checkNumberOperands(const Token& op,
        const any& left, const any& right);
    std::string stringify(const any& obj);

    std::unique_ptr<Environment> environment;
};

}