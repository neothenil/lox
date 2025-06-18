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

std::unique_ptr<Expr> Parser::expression()
{
    return equality();
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
    if (match({TokenType::FALSE})) {
        return std::make_unique<Literal>(std::make_unique<any>(false));
    }
    if (match({TokenType::TRUE})) {
        return std::make_unique<Literal>(std::make_unique<any>(true));
    }
    if (match({TokenType::NIL})) {
        return std::make_unique<Literal>(std::make_unique<any>(nullptr));
    }
    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(
            std::make_unique<any>(previous().literal));
    }

    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

std::unique_ptr<Expr> Parser::parse()
{
    try {
        return expression();
    }
    catch (const ParserError& error) {
        return nullptr;
    }
}

}