#include <stdexcept>
#include <unordered_set>

#include <scriptum/frontend/parser/expression_parser.hpp>
#include <scriptum/frontend/lexer/token.hpp>

namespace scriptum {

namespace {

const std::unordered_set<std::string> kAssignmentOperators = {"=", "+=", "-=", "*=", "/="};
const std::unordered_set<std::string> kEqualityOperators = {"===", "!=="};
const std::unordered_set<std::string> kComparisonOperators = {">", "<", ">=", "<="};
const std::unordered_set<std::string> kAdditiveOperators = {"+", "-"};
const std::unordered_set<std::string> kMultiplicativeOperators = {"*", "/", "%"};

} // namespace

ExpressionParser::ExpressionParser(ParserContext& ctx) : m_ctx(ctx) {}

ExprPtr ExpressionParser::parseExpression()
{
    return parseAssignmentExpression();
}

ExprPtr ExpressionParser::parseAssignmentExpression()
{
    auto left = parseLogicalOrExpression();

    if (kAssignmentOperators.contains(m_ctx.peek().value))
    {
        const Identifier* const id = std::get_if<Identifier>(&left->node);
        if (id == nullptr)
        {
            throw std::runtime_error(
                "Invalid syntax, cannot assign a expression to anything other than identifier");
        }

        const std::string op = m_ctx.eat().value;
        return std::make_unique<Expression>(std::in_place_type<AssignmentExpression>, *id,
                                            parseAssignmentExpression(), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseLogicalOrExpression()
{
    auto left = parseLogicalAndExpression();

    while (m_ctx.peek().value == "||")
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseLogicalAndExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseLogicalAndExpression()
{
    auto left = parseEqualityExpression();

    while (m_ctx.peek().value == "&&")
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseEqualityExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseEqualityExpression()
{
    auto left = parseComparisonExpression();

    while (kEqualityOperators.contains(m_ctx.peek().value))
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseComparisonExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseComparisonExpression()
{
    auto left = parseAdditiveExpression();

    while (kComparisonOperators.contains(m_ctx.peek().value))
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseAdditiveExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseAdditiveExpression()
{
    auto left = parseMultiplicativeExpression();

    while (kAdditiveOperators.contains(m_ctx.peek().value))
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseMultiplicativeExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseMultiplicativeExpression()
{
    auto left = parseCallExpression();

    while (kMultiplicativeOperators.contains(m_ctx.peek().value))
    {
        const std::string op = m_ctx.eat().value;
        auto right = parseCallExpression();
        left = std::make_unique<Expression>(std::in_place_type<BinaryExpression>, std::move(left),
                                            std::move(right), op);
    }

    return left;
}

ExprPtr ExpressionParser::parseCallExpression()
{
    auto expr = parsePrimaryExpression();

    while (m_ctx.peek().type == TokenType::OpenParenthesis)
    {
        m_ctx.eat();
        auto args = parseArguments();
        expr = std::make_unique<Expression>(std::in_place_type<CallExpression>, std::move(expr),
                                            std::move(args));

        if (m_ctx.peek().type == TokenType::ClosedParenthesis)
        {
            m_ctx.eat();
        }
        else
        {
            throw std::runtime_error("No matching closed parenthesis");
        }
    }

    return expr;
}

ExprPtr ExpressionParser::parsePrimaryExpression()
{
    switch (m_ctx.peek().type)
    {
        case TokenType::Number:
            return parseNumericLiteral();
        case TokenType::Identifier: {
            Identifier id = parseIdentifier();
            return std::make_unique<Expression>(std::in_place_type<Identifier>,
                                                std::move(id.symbol));
        }
        case TokenType::OpenParenthesis:
            return parseGroupedExpression();
        case TokenType::Eof:
            throw std::runtime_error("Erorr!, Invalid primary expression in statement");
        default:
            throw std::runtime_error("Error!, Could not identify symbol '" + m_ctx.eat().value +
                                     "'");
    }
}

Identifier ExpressionParser::parseIdentifier()
{
    if (m_ctx.peek().type != TokenType::Identifier)
    {
        throw std::runtime_error("Expected identifier");
    }
    return Identifier(m_ctx.eat().value);
}

ExprPtr ExpressionParser::parseNumericLiteral()
{
    if (m_ctx.peek().type != TokenType::Number)
    {
        throw std::runtime_error("Expected number");
    }
    return std::make_unique<Expression>(std::in_place_type<NumericLiteral>,
                                        std::stod(m_ctx.eat().value));
}

ExprPtr ExpressionParser::parseGroupedExpression()
{
    if (m_ctx.peek().type != TokenType::OpenParenthesis)
    {
        throw std::runtime_error("Expected open parenthesis");
    }

    m_ctx.eat();
    auto expr = parseExpression();

    if (m_ctx.peek().type != TokenType::ClosedParenthesis)
    {
        throw std::runtime_error("Non-matching parenthesis");
    }

    m_ctx.eat();
    return expr;
}

std::vector<ExprPtr> ExpressionParser::parseArguments()
{
    std::vector<ExprPtr> args;

    if (m_ctx.peek().type == TokenType::ClosedParenthesis)
    {
        return args;
    }

    args.push_back(parseExpression());

    while (!m_ctx.endOf() && m_ctx.peek().type == TokenType::Comma)
    {
        m_ctx.eat();
        args.push_back(parseExpression());
    }

    if (m_ctx.peek().type != TokenType::ClosedParenthesis)
    {
        throw std::runtime_error("No matching closed parenthesis");
    }

    return args;
}

} // namespace scriptum
