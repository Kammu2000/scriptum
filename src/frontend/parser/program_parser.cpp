#include <scriptum/frontend/lexer/token.hpp>
#include <scriptum/frontend/parser/program_parser.hpp>
#include <scriptum/frontend/parser/expression_parser.hpp>
#include <scriptum/frontend/parser/parser_context.hpp>
#include <scriptum/frontend/parser/statement_parser.hpp>

namespace scriptum {

Program ProgramParser::buildAST(const std::string& sourceCode)
{
    Program program;

    const std::vector<Token> tokens = tokenize(sourceCode);
    ParserContext parserContext(tokens, 0);
    ExpressionParser expressionParser(parserContext);
    StatementParser statementParser(parserContext, expressionParser);

    while (!parserContext.endOf())
    {
        program.body.push_back(statementParser.parseStatement());
    }

    return program;
}

} // namespace scriptum
