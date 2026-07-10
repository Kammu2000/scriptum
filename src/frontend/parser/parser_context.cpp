#include <scriptum/frontend/parser/parser_context.hpp>

namespace scriptum {

ParserContext::ParserContext(std::vector<Token> tokens, std::size_t pos)
    : m_tokens(std::move(tokens)), m_pos(pos)
{
}

const Token& ParserContext::peek() const
{
    return m_tokens[m_pos];
}

const Token& ParserContext::next() const
{
    return m_tokens[m_pos + 1];
}

Token ParserContext::eat()
{
    return m_tokens[m_pos++];
}

bool ParserContext::endOf() const
{
    return peek().type == TokenType::Eof;
}

} // namespace scriptum
