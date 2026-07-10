#include <stdexcept>

#include <scriptum/frontend/parser/statement_parser.hpp>
#include <scriptum/frontend/lexer/token.hpp>

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
            return Statement::makeBlockStatement(std::move(block->body));
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
        throw std::runtime_error("Variable declaration should contain identifier after let");
    }

    const Identifier identifier = m_expressionParser.parseIdentifier();
    std::optional<ExprPtr> init;

    if (m_ctx.peek().type == TokenType::Equals)
    {
        m_ctx.eat();
        init = m_expressionParser.parseExpression();
    }

    return Statement::makeVariableDeclaration(identifier, std::move(init));
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
            elseBlock = Statement::makeBlockStatement(std::move(block->body));
        }
        else
        {
            throw std::runtime_error("else statement should have block or a new if statement");
        }
    }

    return Statement::makeIfStatement(std::move(test), std::move(thenBlock), std::move(elseBlock));
}

BlockStmtPtr StatementParser::parseBlockStatement()
{
    if (m_ctx.peek().type != TokenType::OpenBraces)
    {
        throw std::runtime_error("No open braces found at staring of in block statement");
    }
    m_ctx.eat();

    std::vector<StmtPtr> body;

    while (!m_ctx.endOf() && m_ctx.peek().type != TokenType::ClosedBraces)
    {
        body.push_back(parseStatement());
    }

    if (m_ctx.peek().type != TokenType::ClosedBraces)
    {
        throw std::runtime_error("No matching closed braces for open braces");
    }
    m_ctx.eat();

    auto block = std::make_unique<BlockStatement>();
    block->body = std::move(body);
    return block;
}

StmtPtr StatementParser::parseWhileStatement()
{
    m_ctx.eat();
    ExprPtr test = m_expressionParser.parseGroupedExpression();
    BlockStmtPtr loopBody = parseBlockStatement();
    return Statement::makeWhileStatement(std::move(test), std::move(loopBody));
}

StmtPtr StatementParser::parseReturnStatement()
{
    m_ctx.eat();

    if (m_ctx.peek().type == TokenType::ClosedBraces)
    {
        return Statement::makeReturnStatement(std::nullopt);
    }

    return Statement::makeReturnStatement(m_expressionParser.parseExpression());
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
        throw std::runtime_error("function definition is not correct, no parenthesis after name");
    }
    m_ctx.eat();

    std::vector<Identifier> params;

    while (m_ctx.peek().type != TokenType::ClosedParenthesis)
    {
        if (m_ctx.peek().type != TokenType::Identifier)
        {
            throw std::runtime_error("function parameter is not an identifier");
        }

        params.push_back(m_expressionParser.parseIdentifier());

        if (m_ctx.peek().type == TokenType::Comma)
        {
            if (m_ctx.next().type == TokenType::ClosedParenthesis)
            {
                throw std::runtime_error(
                    "Trailing comma is not allowed after all function parameters");
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
        throw std::runtime_error(
            "No matching closed parenthesis was found in function declaration");
    }

    BlockStmtPtr body = parseBlockStatement();
    return Statement::makeFunctionDeclaration(id, std::move(params), std::move(body));
}

StmtPtr StatementParser::parseExpressionStatement()
{
    return Statement::makeExpressionStatement(m_expressionParser.parseExpression());
}

} // namespace scriptum
