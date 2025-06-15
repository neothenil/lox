#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <any>
#include <map>
#include <magic_enum/magic_enum.hpp>

namespace lox {

enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    // EOF is defined as a macro expanded to (-1) by standard library.
    // So we use TOKEN_EOF here.
    TOKEN_EOF
};

class Token
{
public:
    Token(TokenType type, const std::string& lexeme,
        const std::any& literal, int line):
        type(type), lexeme(lexeme), literal(literal), line(line) {}
    Token(const Token& other) { *this = other; }
    Token& operator=(const Token& other)
    {
        type = other.type;
        lexeme = other.lexeme;
        literal = other.literal;
        line = other.line;
        return *this;
    }

// private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    friend std::ostream& operator<<(std::ostream&, const Token&);
};

inline std::ostream& operator<<(std::ostream& out, const Token& token)
{
    out << "type: " << magic_enum::enum_name(token.type);
    out << ", lexeme: " << token.lexeme;
    out << ", has_literal: "
        << (token.literal.has_value() ? "true" : "false");
    return out;
}

class Scanner
{
public:
    Scanner(const std::string& source): mSource(source) {}

    std::vector<Token> scanTokens();

// private:
    bool isAtEnd() const { return mCurrent >= mSource.length(); }
    void scanToken();
    char advance() { return mSource[mCurrent++]; }
    void addToken(TokenType type) { addToken(type, std::any()); }
    void addToken(TokenType type, const std::any& literal)
    {
        auto text = mSource.substr(mStart, mCurrent - mStart);
        mTokens.push_back(Token(type, text, literal, mLine));
    }
    bool match(char expected)
    {
        if (isAtEnd()) return false;
        if (mSource[mCurrent] != expected) return false;
        ++mCurrent;
        return true;
    }
    char peek() const
    {
        if (isAtEnd()) return '\0';
        return mSource[mCurrent];
    }
    char peekNext() const
    {
        if (mCurrent + 1 >= mSource.length()) return '\0';
        return mSource[mCurrent + 1];
    }
    bool isDigit(char c) const { return c >= '0' && c <= '9'; }
    bool isAlpha(char c) const
    {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }
    bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

    void string();
    void number();
    void identifier();

private:
    std::string mSource;
    std::vector<Token> mTokens;
    size_t mStart = 0;
    size_t mCurrent = 0;
    size_t mLine = 1;

    static const std::map<std::string, TokenType> sKeywords;
};

}