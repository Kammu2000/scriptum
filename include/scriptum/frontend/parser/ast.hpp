#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace scriptum {

struct Expression;
struct Statement;
struct BlockStatement;

using ExprPtr = std::unique_ptr<Expression>;
using StmtPtr = std::unique_ptr<Statement>;
using BlockStmtPtr = std::unique_ptr<BlockStatement>;

struct NumericLiteral
{
    explicit NumericLiteral(double value);

    double value = 0.0;
};

struct Identifier
{
    explicit Identifier(std::string symbol);

    std::string symbol;
};

struct BinaryExpression
{
    BinaryExpression(ExprPtr&& left, ExprPtr&& right, std::string op);

    ExprPtr left;
    ExprPtr right;
    std::string op;
};

struct AssignmentExpression
{
    AssignmentExpression(Identifier left, ExprPtr&& right, std::string op);

    Identifier left;
    ExprPtr right;
    std::string op;
};

struct CallExpression
{
    CallExpression(ExprPtr&& callee, std::vector<ExprPtr> args);

    ExprPtr callee;
    std::vector<ExprPtr> args;
};

struct Expression
{
    std::variant<NumericLiteral, Identifier, BinaryExpression, AssignmentExpression, CallExpression>
        node;

    template <typename T, typename... Args>
    explicit Expression(std::in_place_type_t<T>, Args&&... args)
        : node(std::in_place_type<T>, std::forward<Args>(args)...)
    {
    }
};

struct ExpressionStatement
{
    explicit ExpressionStatement(ExprPtr&& expression);

    ExprPtr expression;
};

struct VariableDeclaration
{
    VariableDeclaration(Identifier id, std::optional<ExprPtr> init);

    Identifier id;
    std::optional<ExprPtr> init;
};

struct BlockStatement
{
    explicit BlockStatement(std::vector<StmtPtr> body);

    std::vector<StmtPtr> body;
};

struct IfStatement
{
    IfStatement(ExprPtr&& test, BlockStmtPtr&& thenBlock, StmtPtr&& elseBlock);

    ExprPtr test;
    BlockStmtPtr thenBlock;
    StmtPtr elseBlock;
};

struct WhileStatement
{
    WhileStatement(ExprPtr&& test, BlockStmtPtr&& body);

    ExprPtr test;
    BlockStmtPtr body;
};

struct ReturnStatement
{
    explicit ReturnStatement(std::optional<ExprPtr> argument);

    std::optional<ExprPtr> argument;
};

struct FunctionDeclaration
{
    FunctionDeclaration(Identifier id, std::vector<Identifier> params, BlockStmtPtr&& body);

    Identifier id;
    std::vector<Identifier> params;
    BlockStmtPtr body;
};

struct Statement
{
    std::variant<ExpressionStatement, VariableDeclaration, IfStatement, WhileStatement,
                 BlockStatement, ReturnStatement, FunctionDeclaration>
        node;

    template <typename T, typename... Args>
    explicit Statement(std::in_place_type_t<T>, Args&&... args)
        : node(std::in_place_type<T>, std::forward<Args>(args)...)
    {
    }
};

struct Program
{
    std::vector<StmtPtr> body;
};

} // namespace scriptum
