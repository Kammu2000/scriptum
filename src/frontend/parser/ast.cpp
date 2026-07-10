#include <scriptum/frontend/parser/ast.hpp>

namespace scriptum {

ExprPtr Expression::makeNumericLiteral(double value)
{
    auto expr = std::make_unique<Expression>();
    expr->kind = ExpressionKind::NumericLiteral;
    expr->numericLiteral.value = value;
    return expr;
}

ExprPtr Expression::makeIdentifier(std::string symbol)
{
    auto expr = std::make_unique<Expression>();
    expr->kind = ExpressionKind::Identifier;
    expr->identifier.symbol = std::move(symbol);
    return expr;
}

ExprPtr Expression::makeBinaryExpression(ExprPtr&& left, ExprPtr&& right, std::string op)
{
    auto expr = std::make_unique<Expression>();
    expr->kind = ExpressionKind::BinaryExpression;
    expr->binaryExpression.left = std::move(left);
    expr->binaryExpression.right = std::move(right);
    expr->binaryExpression.op = std::move(op);
    return expr;
}

ExprPtr Expression::makeAssignmentExpression(Identifier left, ExprPtr&& right, std::string op)
{
    auto expr = std::make_unique<Expression>();
    expr->kind = ExpressionKind::AssignmentExpression;
    expr->assignmentExpression.left = std::move(left);
    expr->assignmentExpression.right = std::move(right);
    expr->assignmentExpression.op = std::move(op);
    return expr;
}

ExprPtr Expression::makeCallExpression(ExprPtr&& callee, std::vector<ExprPtr> args)
{
    auto expr = std::make_unique<Expression>();
    expr->kind = ExpressionKind::CallExpression;
    expr->callExpression.callee = std::move(callee);
    expr->callExpression.args = std::move(args);
    return expr;
}

StmtPtr Statement::makeExpressionStatement(ExprPtr&& expression)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::ExpressionStatement;
    stmt->expressionStatement.expression = std::move(expression);
    return stmt;
}

StmtPtr Statement::makeVariableDeclaration(Identifier id, std::optional<ExprPtr> init)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::VariableDeclaration;
    stmt->variableDeclaration.id = std::move(id);
    stmt->variableDeclaration.init = std::move(init);
    return stmt;
}

StmtPtr Statement::makeIfStatement(ExprPtr&& test, BlockStmtPtr&& thenBlock, StmtPtr&& elseBlock)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::IfStatement;
    stmt->ifStatement.test = std::move(test);
    stmt->ifStatement.thenBlock = std::move(thenBlock);
    stmt->ifStatement.elseBlock = std::move(elseBlock);
    return stmt;
}

StmtPtr Statement::makeWhileStatement(ExprPtr&& test, BlockStmtPtr&& body)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::WhileStatement;
    stmt->whileStatement.test = std::move(test);
    stmt->whileStatement.body = std::move(body);
    return stmt;
}

StmtPtr Statement::makeBlockStatement(std::vector<StmtPtr> body)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::BlockStatement;
    stmt->blockStatement.body = std::move(body);
    return stmt;
}

StmtPtr Statement::makeReturnStatement(std::optional<ExprPtr> argument)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::ReturnStatement;
    stmt->returnStatement.argument = std::move(argument);
    return stmt;
}

StmtPtr Statement::makeFunctionDeclaration(Identifier id, std::vector<Identifier> params,
                                           BlockStmtPtr&& body)
{
    auto stmt = std::make_unique<Statement>();
    stmt->kind = StatementKind::FunctionDeclaration;
    stmt->functionDeclaration.id = std::move(id);
    stmt->functionDeclaration.params = std::move(params);
    stmt->functionDeclaration.body = std::move(body);
    return stmt;
}

} // namespace scriptum
