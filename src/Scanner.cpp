#include "Scanner.h"
#include "Lox.h"

namespace lox {

const std::map<std::string, TokenType> Scanner::sKeywords {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE}
};

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd()) {
        mStart = mCurrent;
        scanToken();
    }
    mTokens.push_back(Token(TokenType::TOKEN_EOF, "", std::any(), mLine));
    return mTokens;
}

void Scanner::scanToken()
{
    char c = advance();
    switch (c) {
    // single character tokens
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break;
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break;
    // operators
    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of line.
            while (peek() != '\n' && !isAtEnd()) advance();
        }
        else {
            addToken(TokenType::SLASH);
        }
        break;
    // whitespaces
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        ++mLine;
        break;
    case '"':
        string();
        break;
    default:
        if (isDigit(c)) {
            number();
        }
        else if (isAlpha(c)) {
            identifier();
        }
        else {
            error(mLine, "Unexpected character.");
        }
        break;
    }
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') ++mLine;
        advance();
    }

    if (isAtEnd()) {
        error(mLine, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = mSource.substr(mStart + 1, mCurrent - mStart - 2);
    addToken(TokenType::STRING, value);
}

void Scanner::number()
{
    while (isDigit(peek())) {
        advance();
    }
    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();
        while (isDigit(peek())) advance();
    }
    addToken(TokenType::NUMBER, std::stod(mSource.substr(mStart, mCurrent - mStart)));
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek())) advance();
    std::string text = mSource.substr(mStart, mCurrent - mStart);
    TokenType type = TokenType::IDENTIFIER;
    if (sKeywords.find(text) != sKeywords.end()) {
        type = sKeywords.at(text);
        switch (type) {
        case TokenType::FALSE:
            addToken(type, false);
            return;
        case TokenType::TRUE:
            addToken(type, true);
            return;
        case TokenType::NIL:
            addToken(type, nullptr);
            return;
        default:
            break;
        }
    }
    addToken(type);
}

}