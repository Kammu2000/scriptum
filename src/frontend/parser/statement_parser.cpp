#include <format>
#include <memory>
#include <stdexcept>
#include <utility>

#include <scriptum/frontend/parser/statement_parser.hpp>
#include <scriptum/frontend/lexer/token.hpp>
#include "scriptum/frontend/parser/ast.hpp"

namespace scriptum {

StatementParser::StatementParser(ParserContext& ctx, ExpressionParser& expressionParser)
    : m_ctx(ctx), m_expressionParser(expressionParser)
{
}

StmtPtr StatementParser::parseStatement()
{
    switch (m_ctx.peek().type)
    {
        case TokenType::Let:
            return parseVarDeclaration();
        case TokenType::If:
            return parseIfStatement();
        case TokenType::While:
            return parseWhileStatement();
        case TokenType::OpenBraces: {
            BlockStmtPtr block = parseBlockStatement();
            return std::make_unique<Statement>(std::in_place_type<BlockStatement>,
                                               std::move(block->body));
        }
        case TokenType::Return:
            return parseReturnStatement();
        case TokenType::Function:
            return parseFunctionDeclaration();
        default:
            return parseExpressionStatement();
    }
}

StmtPtr StatementParser::parseVarDeclaration()
{
    m_ctx.eat();

    if (m_ctx.peek().type != TokenType::Identifier)
    {
        throw std::runtime_error(
            std::format("No identifier in variable declaration after let at: {}", m_ctx.getPos()));
    }

    const Identifier identifier = m_expressionParser.parseIdentifier();
    std::optional<ExprPtr> init;

    if (m_ctx.peek().type == TokenType::Equals)
    {
        m_ctx.eat();
        init = m_expressionParser.parseExpression();
    }

    return std::make_unique<Statement>(std::in_place_type<VariableDeclaration>,
                                       std::move(identifier), std::move(init));
}

StmtPtr StatementParser::parseIfStatement()
{
    m_ctx.eat();

    ExprPtr test = m_expressionParser.parseGroupedExpression();
    BlockStmtPtr thenBlock = parseBlockStatement();
    StmtPtr elseBlock;

    if (m_ctx.peek().type == TokenType::Else)
    {
        m_ctx.eat();
        if (m_ctx.peek().type == TokenType::If)
        {
            elseBlock = parseIfStatement();
        }
        else if (m_ctx.peek().type == TokenType::OpenBraces)
        {
            BlockStmtPtr block = parseBlockStatement();
            elseBlock = std::make_unique<Statement>(std::in_place_type<BlockStatement>,
                                                    std::move(block->body));
        }
        else
        {
            throw std::runtime_error(
                std::format("No block or if statement along with else at: ", m_ctx.getPos()));
        }
    }

    return std::make_unique<Statement>(std::in_place_type<IfStatement>, std::move(test),
                                       std::move(thenBlock), std::move(elseBlock));
}

BlockStmtPtr StatementParser::parseBlockStatement()
{
    if (m_ctx.peek().type != TokenType::OpenBraces)
    {
        throw std::runtime_error(std::format(
            "No open braces found in staring of block statement at: {}", m_ctx.getPos()));
    }
    m_ctx.eat();

    std::vector<StmtPtr> body;

    while (!m_ctx.endOf() && m_ctx.peek().type != TokenType::ClosedBraces)
    {
        body.push_back(parseStatement());
    }

    if (m_ctx.peek().type != TokenType::ClosedBraces)
    {
        throw std::runtime_error(
            std::format("No matching closed braces for open braces at: {}", m_ctx.getPos()));
    }
    m_ctx.eat();

    return std::make_unique<BlockStatement>(std::move(body));
}

StmtPtr StatementParser::parseWhileStatement()
{
    m_ctx.eat();
    ExprPtr test = m_expressionParser.parseGroupedExpression();
    BlockStmtPtr loopBody = parseBlockStatement();
    return std::make_unique<Statement>(std::in_place_type<WhileStatement>, std::move(test),
                                       std::move(loopBody));
}

StmtPtr StatementParser::parseReturnStatement()
{
    m_ctx.eat();

    if (m_ctx.peek().type == TokenType::ClosedBraces)
    {
        return std::make_unique<Statement>(std::in_place_type<ReturnStatement>, std::nullopt);
    }

    return std::make_unique<Statement>(std::in_place_type<ReturnStatement>,
                                       m_expressionParser.parseExpression());
}

StmtPtr StatementParser::parseFunctionDeclaration()
{
    m_ctx.eat();

    if (m_ctx.peek().type != TokenType::Identifier)
    {
        throw std::runtime_error("Anonymous functions are not allowed in this language");
    }

    const Identifier id = m_expressionParser.parseIdentifier();

    if (m_ctx.peek().type != TokenType::OpenParenthesis)
    {
        throw std::runtime_error(
            std::format("function definition is not correct, no parenthesis after: {}", id.symbol));
    }
    m_ctx.eat();

    std::vector<Identifier> params;

    while (m_ctx.peek().type != TokenType::ClosedParenthesis)
    {
        if (m_ctx.peek().type != TokenType::Identifier)
        {
            throw std::runtime_error(
                std::format("function parameter is not an identifier for function: {}", id.symbol));
        }

        params.emplace_back(m_expressionParser.parseIdentifier());

        if (m_ctx.peek().type == TokenType::Comma)
        {
            if (m_ctx.next().type == TokenType::ClosedParenthesis)
            {
                throw std::runtime_error(std::format(
                    "Trailing comma after all function parameters for function: {}", id.symbol));
            }
            m_ctx.eat();
        }
        else
        {
            break;
        }
    }

    if (m_ctx.eat().type != TokenType::ClosedParenthesis)
    {
        throw std::runtime_error(std::format(
            "No matching closed parenthesis was found in function declaration of: {}", id.symbol));
    }

    BlockStmtPtr body = parseBlockStatement();
    return std::make_unique<Statement>(std::in_place_type<FunctionDeclaration>, std::move(id),
                                       std::move(params), std::move(body));
}

StmtPtr StatementParser::parseExpressionStatement()
{
    return std::make_unique<Statement>(std::in_place_type<ExpressionStatement>,
                                       m_expressionParser.parseExpression());
}

} // namespace scriptum
