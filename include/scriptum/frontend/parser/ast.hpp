#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace scriptum {

struct Expression;
struct Statement;
struct BlockStatement;

using ExprPtr = std::unique_ptr<Expression>;
using StmtPtr = std::unique_ptr<Statement>;
using BlockStmtPtr = std::unique_ptr<BlockStatement>;

enum class ExpressionKind
{
    NumericLiteral,
    Identifier,
    BinaryExpression,
    AssignmentExpression,
    CallExpression,
};

enum class StatementKind
{
    VariableDeclaration,
    IfStatement,
    WhileStatement,
    ExpressionStatement,
    ReturnStatement,
    FunctionDeclaration,
    BlockStatement,
};

struct NumericLiteral
{
    double value = 0.0;
};

struct Identifier
{
    std::string symbol;
};

struct BinaryExpression
{
    ExprPtr left;
    ExprPtr right;
    std::string op;
};

struct AssignmentExpression
{
    Identifier left;
    ExprPtr right;
    std::string op;
};

struct CallExpression
{
    ExprPtr callee;
    std::vector<ExprPtr> args;
};

struct Expression
{
    ExpressionKind kind;
    NumericLiteral numericLiteral;
    Identifier identifier;
    BinaryExpression binaryExpression;
    AssignmentExpression assignmentExpression;
    CallExpression callExpression;

    static ExprPtr makeNumericLiteral(double value);
    static ExprPtr makeIdentifier(std::string symbol);
    static ExprPtr makeBinaryExpression(ExprPtr&& left, ExprPtr&& right, std::string op);
    static ExprPtr makeAssignmentExpression(Identifier left, ExprPtr&& right, std::string op);
    static ExprPtr makeCallExpression(ExprPtr&& callee, std::vector<ExprPtr> args);
};

struct ExpressionStatement
{
    ExprPtr expression;
};

struct VariableDeclaration
{
    Identifier id;
    std::optional<ExprPtr> init;
};

struct BlockStatement
{
    std::vector<StmtPtr> body;
};

struct IfStatement
{
    ExprPtr test;
    BlockStmtPtr thenBlock;
    StmtPtr elseBlock;
};

struct WhileStatement
{
    ExprPtr test;
    BlockStmtPtr body;
};

struct ReturnStatement
{
    std::optional<ExprPtr> argument;
};

struct FunctionDeclaration
{
    Identifier id;
    std::vector<Identifier> params;
    BlockStmtPtr body;
};

struct Statement
{
    StatementKind kind;
    ExpressionStatement expressionStatement;
    VariableDeclaration variableDeclaration;
    IfStatement ifStatement;
    WhileStatement whileStatement;
    BlockStatement blockStatement;
    ReturnStatement returnStatement;
    FunctionDeclaration functionDeclaration;

    static StmtPtr makeExpressionStatement(ExprPtr&& expression);
    static StmtPtr makeVariableDeclaration(Identifier id, std::optional<ExprPtr> init);
    static StmtPtr makeIfStatement(ExprPtr&& test, BlockStmtPtr&& thenBlock, StmtPtr&& elseBlock);
    static StmtPtr makeWhileStatement(ExprPtr&& test, BlockStmtPtr&& body);
    static StmtPtr makeBlockStatement(std::vector<StmtPtr> body);
    static StmtPtr makeReturnStatement(std::optional<ExprPtr> argument);
    static StmtPtr makeFunctionDeclaration(Identifier id, std::vector<Identifier> params,
                                           BlockStmtPtr&& body);
};

struct Program
{
    std::vector<StmtPtr> body;
};

} // namespace scriptum
