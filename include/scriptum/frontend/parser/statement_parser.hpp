#pragma once

#include <scriptum/frontend/parser/ast.hpp>
#include <scriptum/frontend/parser/expression_parser.hpp>
#include <scriptum/frontend/parser/parser_context.hpp>

namespace scriptum {

class StatementParser
{
  public:
    StatementParser(ParserContext& ctx, ExpressionParser& expressionParser);

    StmtPtr parseStatement();

  private:
    StmtPtr parseVarDeclaration();
    StmtPtr parseIfStatement();
    BlockStmtPtr parseBlockStatement();
    StmtPtr parseWhileStatement();
    StmtPtr parseReturnStatement();
    StmtPtr parseFunctionDeclaration();
    StmtPtr parseExpressionStatement();

    ParserContext& m_ctx;
    ExpressionParser& m_expressionParser;
};

} // namespace scriptum
