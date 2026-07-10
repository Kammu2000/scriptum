#include <cctype>
#include <format>
#include <stdexcept>
#include <unordered_map>

#include <scriptum/frontend/lexer/token.hpp>

namespace scriptum {

namespace {

const std::unordered_map<std::string, TokenType> kReservedWords = {
    {"let", TokenType::Let},       {"if", TokenType::If},       {"else", TokenType::Else},
    {"for", TokenType::For},       {"while", TokenType::While}, {"function", TokenType::Function},
    {"return", TokenType::Return},
};

} // namespace

std::vector<Token> tokenize(const std::string& sourceCode)
{
    std::vector<Token> tokens;
    const std::size_t srcLen = sourceCode.length();
    std::size_t idx = 0;

    auto getNumberToken = [&]() -> Token {
        std::string num;

        while (idx < srcLen && std::isdigit(static_cast<unsigned char>(sourceCode[idx])))
        {
            num += sourceCode[idx];
            idx++;
        }

        return {TokenType::Number, num};
    };

    auto getWordToken = [&]() -> Token {
        std::string word;

        while (idx < srcLen && (std::isalnum(static_cast<unsigned char>(sourceCode[idx])) ||
                                sourceCode[idx] == '_'))
        {
            word += sourceCode[idx];
            idx++;
        }

        const auto it = kReservedWords.find(word);

        if (it != kReservedWords.end())
        {
            return {it->second, word};
        }

        return {TokenType::Identifier, word};
    };

    while (idx < srcLen)
    {
        switch (sourceCode[idx])
        {
            case '(': {
                tokens.push_back({TokenType::OpenParenthesis, std::string(1, sourceCode[idx++])});
                break;
            }

            case ')': {
                tokens.push_back({TokenType::ClosedParenthesis, std::string(1, sourceCode[idx++])});
                break;
            }

            case '{': {
                tokens.push_back({TokenType::OpenBraces, std::string(1, sourceCode[idx++])});
                break;
            }

            case '}': {
                tokens.push_back({TokenType::ClosedBraces, std::string(1, sourceCode[idx++])});
                break;
            }

            case '!': {
                if (idx + 2 < srcLen && sourceCode[idx + 1] == '=' && sourceCode[idx + 2] == '=')
                {
                    tokens.push_back({TokenType::BinaryOperator, "!=="});
                    idx += 3;
                }
                else
                {
                    throw std::runtime_error(
                        std::format("Unknown character group starting from ! at: {}", idx));
                }

                break;
            }

            case '>': {
                if (idx + 1 < srcLen && sourceCode[idx + 1] == '=')
                {
                    tokens.push_back({TokenType::BinaryOperator, ">="});
                    idx += 2;
                }
                else
                {
                    tokens.push_back({TokenType::BinaryOperator, ">"});
                    idx += 1;
                }

                break;
            }

            case '<': {
                if (idx + 1 < srcLen && sourceCode[idx + 1] == '=')
                {
                    tokens.push_back({TokenType::BinaryOperator, "<="});
                    idx += 2;
                }
                else
                {
                    tokens.push_back({TokenType::BinaryOperator, "<"});
                    idx += 1;
                }

                break;
            }

            case '%':
            case '+':
            case '-':
            case '*':
            case '/': {
                tokens.push_back({TokenType::BinaryOperator, std::string(1, sourceCode[idx++])});
                break;
            }

            case ',': {
                tokens.push_back({TokenType::Comma, std::string(1, sourceCode[idx++])});
                break;
            }

            case ' ':
            case '\n':
            case '\t': {
                idx++;
                break;
            }

            case '=': {
                if (idx + 2 < srcLen && sourceCode[idx + 1] == '=' && sourceCode[idx + 2] == '=')
                {
                    tokens.push_back({TokenType::BinaryOperator, "==="});
                    idx += 3;
                }
                else if (idx + 1 < srcLen && sourceCode[idx + 1] == '=')
                {
                    throw std::runtime_error(std::format("Invalid character group == at: {}", idx));
                }
                else
                {
                    tokens.push_back({TokenType::Equals, "="});
                    idx += 1;
                }

                break;
            }

            case '&': {
                if (idx + 1 < srcLen && sourceCode[idx + 1] == '&')
                {
                    tokens.push_back({TokenType::BinaryOperator, "&&"});
                    idx += 2;
                }
                else
                {
                    throw std::runtime_error(std::format("Unknown character & at: {}", idx));
                }

                break;
            }

            case '|': {
                if (idx + 1 < srcLen && sourceCode[idx + 1] == '|')
                {
                    tokens.push_back({TokenType::BinaryOperator, "||"});
                    idx += 2;
                }
                else
                {
                    throw std::runtime_error(std::format("Unknown character | at: {}", idx));
                }

                break;
            }

            default: {
                if (std::isdigit(static_cast<unsigned char>(sourceCode[idx])))
                {
                    tokens.push_back(getNumberToken());
                }
                else if (std::isalpha(static_cast<unsigned char>(sourceCode[idx])) ||
                         sourceCode[idx] == '_')
                {
                    tokens.push_back(getWordToken());
                }
                else
                {
                    throw std::runtime_error(
                        std::format("Unknown character {} at: {}", sourceCode[idx], idx));
                }

                break;
            }
        }
    }

    tokens.push_back({TokenType::Eof, "EOF"});
    return tokens;
}

} // namespace scriptum
