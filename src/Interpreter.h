#pragma once

#include "autogen/Expr.h"
#include "autogen/Stmt.h"
#include "Environment.h"
#include <stdexcept>
#include <map>

namespace lox {

class RuntimeError: public std::runtime_error
{
public:
    RuntimeError(const Token& token, const std::string& message):
        std::runtime_error(message), token(token) {}

    Token token;
};

class NativeCallable;
class LoxFunction;

class Interpreter: public ExprVisitor, public StmtVisitor
{
public:
    Interpreter(): globals(make_globals()), environment(globals) {}
    ~Interpreter() override = default;

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
    any visitVarStmtStmt(VarStmt* stmt) override;
    any visitWhileStmt(While* stmt) override;

    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);

    static std::shared_ptr<Environment> make_globals();

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

    // Use shared_ptr to support closure, even though this cound cause
    // memory leakage. C++ doesn't have GC like JAVA.
    std::shared_ptr<Environment> globals;
    std::shared_ptr<Environment> environment;

    static std::map<std::string, std::unique_ptr<NativeCallable>> nativeFuncs;

    friend class LoxFunction;
};

}