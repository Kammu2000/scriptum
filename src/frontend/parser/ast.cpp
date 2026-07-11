#include <scriptum/frontend/parser/ast.hpp>

namespace scriptum {

NumericLiteral::NumericLiteral(double value) : value(value) {}

Identifier::Identifier(std::string symbol) : symbol(std::move(symbol)) {}

BinaryExpression::BinaryExpression(ExprPtr&& left, ExprPtr&& right, std::string op)
    : left(std::move(left)), right(std::move(right)), op(std::move(op))
{
}

AssignmentExpression::AssignmentExpression(Identifier left, ExprPtr&& right, std::string op)
    : left(std::move(left)), right(std::move(right)), op(std::move(op))
{
}

CallExpression::CallExpression(ExprPtr&& callee, std::vector<ExprPtr> args)
    : callee(std::move(callee)), args(std::move(args))
{
}

ExpressionStatement::ExpressionStatement(ExprPtr&& expression) : expression(std::move(expression)) {}

VariableDeclaration::VariableDeclaration(Identifier id, std::optional<ExprPtr> init)
    : id(std::move(id)), init(std::move(init))
{
}

BlockStatement::BlockStatement(std::vector<StmtPtr> body) : body(std::move(body)) {}

IfStatement::IfStatement(ExprPtr&& test, BlockStmtPtr&& thenBlock, StmtPtr&& elseBlock)
    : test(std::move(test)), thenBlock(std::move(thenBlock)), elseBlock(std::move(elseBlock))
{
}

WhileStatement::WhileStatement(ExprPtr&& test, BlockStmtPtr&& body)
    : test(std::move(test)), body(std::move(body))
{
}

ReturnStatement::ReturnStatement(std::optional<ExprPtr> argument) : argument(std::move(argument)) {}

FunctionDeclaration::FunctionDeclaration(Identifier id, std::vector<Identifier> params,
                                           BlockStmtPtr&& body)
    : id(std::move(id)), params(std::move(params)), body(std::move(body))
{
}

} // namespace scriptum
