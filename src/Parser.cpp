#include "Parser.h"
#include "Lox.h"

namespace lox {

Token Parser::peek() const
{
    return tokens.at(current);
}

bool Parser::isAtEnd() const
{
    return peek().type == TokenType::TOKEN_EOF;
}

Token Parser::previous() const
{
    return tokens.at(current - 1);
}

Token Parser::advance()
{
    if (!isAtEnd()) ++current;
    return previous();
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(std::initializer_list<TokenType> types)
{
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

ParserError Parser::error(const Token& token, const std::string& message)
{
    ::lox::error(token, message);
    return ParserError();
}

Token Parser::consume(TokenType type, const std::string& message)
{
    if (check(type)) return advance();
    throw error(peek(), message);
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

std::unique_ptr<Stmt> Parser::declaration()
{
    try {
        if (match({TokenType::VAR})) return varDeclaration();
        return statement();
    }
    catch (const ParserError& error) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::varDeclaration()
{
    auto name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(
        std::make_unique<Token>(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::statement()
{
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::LEFT_BRACE})) {
        return std::make_unique<Block>(block());
    }
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after print value.");
    return std::make_unique<Print>(std::move(value));
}

std::unique_ptr<vector<unique_ptr<Stmt>>> Parser::block()
{
    auto statements = std::make_unique<vector<unique_ptr<Stmt>>>();

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements->push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Expression>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment()
{
    auto expr = equality();

    if (match({TokenType::EQUAL})) {
        auto equals = previous();
        auto value = assignment();

        if (auto varExpr = dynamic_cast<VarExpr*>(expr.get())) {
            auto name = *varExpr->name;
            return std::make_unique<Assign>(
                std::make_unique<Token>(name), std::move(value)); 
        }

        error(equals, "Invalid assignment target");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality()
{
    auto expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        auto op = std::make_unique<Token>(previous());
        auto right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op),
            std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    auto expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL,
        TokenType::LESS, TokenType::LESS_EQUAL})) {
        auto op = std::make_unique<Token>(previous());
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op),
            std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term()
{
    auto expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        auto op = std::make_unique<Token>(previous());
        auto right = factor();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op),
            std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor()
{
    auto expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        auto op = std::make_unique<Token>(previous());
        auto right = unary();
        expr = std::make_unique<Binary>(std::move(expr), std::move(op),
            std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    if (match({TokenType::BANG, TokenType::MINUS})) {
        auto op = std::make_unique<Token>(previous());
        auto right = unary();
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary()
{
    if (match({TokenType::FALSE, TokenType::TRUE, TokenType::NIL,
               TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(std::make_unique<Token>(previous()));
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<VarExpr>(std::make_unique<Token>(previous()));
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

}