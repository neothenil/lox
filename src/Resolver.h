#pragma once

#include <unordered_map>
#include <vector>

#include "autogen/Expr.h"
#include "autogen/Stmt.h"

namespace lox {

class Interpreter;

class Resolver: public ExprVisitor, public StmtVisitor
{
public:
    explicit Resolver(Interpreter* interpreter): interpreter(interpreter) {}
    ~Resolver() override = default;

    any visitAssignExpr(Assign* expr) override;
    any visitBinaryExpr(Binary* expr) override;
    any visitCallExpr(Call* expr) override;
    any visitGroupingExpr(Grouping* expr) override;
    any visitLiteralExpr(Literal* expr) override;
    any visitLogicalExpr(Logical* expr) override;
    any visitUnaryExpr(Unary* expr) override;
    any visitVarExprExpr(VarExpr* expr) override;

    any visitBlockStmt(Block* stmt) override;
    any visitExpressionStmt(Expression* stmt) override;
    any visitFunctionStmt(Function* stmt) override;
    any visitIfStmt(If* stmt) override;
    any visitPrintStmt(Print* stmt) override;
    any visitReturnStmt(Return* stmt) override;
    any visitVarStmtStmt(VarStmt* stmt) override;
    any visitWhileStmt(While* stmt) override;

    void resolve(std::vector<std::unique_ptr<Stmt>>& statements);

private:
    void resolve(Stmt* stmt);
    void resolve(Expr* expr);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);
    void resolveLocal(Expr* expr, Token& name);
    void resolveFunction(Function* function);

private:
    Interpreter* interpreter = nullptr;
    std::vector<std::unordered_map<std::string, bool>> scopes;
};

}