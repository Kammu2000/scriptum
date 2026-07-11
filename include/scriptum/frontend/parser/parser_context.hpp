#pragma once

#include <cstddef>
#include <vector>

#include <scriptum/frontend/lexer/token.hpp>

namespace scriptum {

class ParserContext
{
  public:
    ParserContext(std::vector<Token> tokens, std::size_t pos);

    std::size_t getPos() const;
    const Token& peek() const;
    const Token& next() const;
    Token eat();
    bool endOf() const;

  private:
    std::vector<Token> m_tokens;
    std::size_t m_pos;
};

} // namespace scriptum
