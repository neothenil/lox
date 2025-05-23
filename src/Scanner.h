#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace lox {

using Token = std::pair<int, std::string>;

inline std::ostream& operator<<(std::ostream& out, const Token& token)
{
    out << "type: " << token.first;
    out << ", lexeme: " << token.second;
    return out;
}

class Scanner
{
public:
    Scanner(const std::string& source): mSource(source) {}

    std::vector<Token> scanTokens() { return {}; }

private:
    std::string mSource;
};

}