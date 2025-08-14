#include "Resolver.h"
#include "Interpreter.h"
#include "Lox.h"

namespace lox {

any Resolver::visitBlockStmt(Block* stmt)
{
    beginScope();
    resolve(*stmt->statements);
    endScope();
    return any();
}

any Resolver::visitExpressionStmt(Expression* stmt)
{
    resolve(stmt->expr.get());
    return any();
}

void Resolver::resolve(std::vector<std::unique_ptr<Stmt>>& statements)
{
    for (auto& statement: statements) {
        resolve(statement.get());
    }
}

void Resolver::resolve(Stmt* stmt)
{
    stmt->accept(this);
}

void Resolver::resolve(Expr* expr)
{
    expr->accept(this);
}

void Resolver::beginScope()
{
    scopes.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::endScope()
{
    scopes.pop_back();
}

any Resolver::visitVarStmtStmt(VarStmt* stmt)
{
    declare(*stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer.get());
    }
    define(*stmt->name);
    return any();
}

void Resolver::declare(Token& name)
{
    if (scopes.empty()) return;

    auto& scope = scopes.back();
    scope[name.lexeme] = false;
}

void Resolver::define(Token& name)
{
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

any Resolver::visitVarExprExpr(VarExpr* expr)
{
    if (!scopes.empty()) {
        auto& scope = scopes.back();
        if (scope.find(expr->name->lexeme) != scope.end()) {
            if (scope.at(expr->name->lexeme) == false) {
                error(*expr->name,
                    "Can't read local variable in its own initializer.");
            }
        }
    }

    resolveLocal(expr, *expr->name);
    return any();
}

void Resolver::resolveLocal(Expr* expr, Token& name)
{
    for (int i = scopes.size() - 1; i >= 0; --i) {
        auto& scope = scopes.at(i);
        if (scope.find(name.lexeme) != scope.end()) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

any Resolver::visitAssignExpr(Assign* expr)
{
    resolve(expr->value.get());
    resolveLocal(expr, *expr->name);
    return any();
}

any Resolver::visitBinaryExpr(Binary* expr)
{
    resolve(expr->left.get());
    resolve(expr->right.get());
    return any();
}

any Resolver::visitCallExpr(Call* expr)
{
    resolve(expr->callee.get());

    for (auto& argument : *expr->arguments) {
        resolve(argument.get());
    }

    return any();
}

any Resolver::visitGroupingExpr(Grouping* expr)
{
    resolve(expr->expression.get());
    return any();
}

any Resolver::visitFunctionStmt(Function* stmt)
{
    declare(*stmt->name);
    define(*stmt->name);

    resolveFunction(stmt);
    return any();
}

any Resolver::visitLiteralExpr(Literal* expr)
{
    return any();
}

any Resolver::visitLogicalExpr(Logical* expr)
{
    resolve(expr->left.get());
    resolve(expr->right.get());
    return any();
}

any Resolver::visitUnaryExpr(Unary* expr)
{
    resolve(expr->right.get());
    return any();
}

void Resolver::resolveFunction(Function* function)
{
    beginScope();
    for (auto& param : *function->params) {
        declare(param);
        define(param);
    }
    resolve(*function->body);
    endScope();
}

any Resolver::visitIfStmt(If* stmt)
{
    resolve(stmt->condition.get());
    resolve(stmt->thenBranch.get());
    if (stmt->elseBranch != nullptr) {
        resolve(stmt->elseBranch.get());
    }
    return any();
}

any Resolver::visitPrintStmt(Print* stmt)
{
    resolve(stmt->expr.get());
    return any();
}

any Resolver::visitReturnStmt(Return* stmt)
{
    if (stmt->value != nullptr) {
        resolve(stmt->value.get());
    }
    return any();
}

any Resolver::visitWhileStmt(While* stmt)
{
    resolve(stmt->condition.get());
    resolve(stmt->body.get());
    return any();
}

}