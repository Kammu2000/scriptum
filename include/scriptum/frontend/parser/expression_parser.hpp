#pragma once

#include <scriptum/frontend/parser/ast.hpp>
#include <scriptum/frontend/parser/parser_context.hpp>

namespace scriptum {

class ExpressionParser
{
  public:
    explicit ExpressionParser(ParserContext& ctx);

    ExprPtr parseExpression();
    ExprPtr parseAssignmentExpression();
    ExprPtr parseLogicalOrExpression();
    ExprPtr parseLogicalAndExpression();
    ExprPtr parseEqualityExpression();
    ExprPtr parseComparisonExpression();
    ExprPtr parseAdditiveExpression();
    ExprPtr parseMultiplicativeExpression();
    ExprPtr parseCallExpression();
    ExprPtr parsePrimaryExpression();
    ExprPtr parseNumericLiteral();
    ExprPtr parseGroupedExpression();
    Identifier parseIdentifier();
    std::vector<ExprPtr> parseArguments();

  private:
    ParserContext& m_ctx;
};

} // namespace scriptum
