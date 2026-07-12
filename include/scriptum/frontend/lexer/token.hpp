#pragma once

#include <string>
#include <vector>

namespace scriptum {

enum class TokenType
{
    Number,
    Identifier,
    OpenParenthesis,
    ClosedParenthesis,
    OpenBraces,
    ClosedBraces,
    BinaryOperator,
    Equals,
    Comma,
    Let,
    If,
    Else,
    While,
    Function,
    Return,
    Eof,
};

struct Token
{
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& sourceCode);

} // namespace scriptum
