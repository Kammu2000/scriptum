#include <variant>

#include <scriptum/overloaded.hpp>
#include <scriptum/runtime/interpreter.hpp>
#include <scriptum/runtime/evaluate_expression.hpp>
#include <scriptum/runtime/return_signal.hpp>
#include <scriptum/runtime/value.hpp>

namespace scriptum {

void executeBlock(const BlockStatement& block, Environment* parentEnv)
{
    Environment childEnv(parentEnv);
    for (const auto& stmt : block.body)
    {
        execute(*stmt, &childEnv);
    }
}

void execute(const Statement& stmt, Environment* env)
{
    std::visit(
        overloaded{
            [env](const ExpressionStatement& node) { evaluateExpression(*node.expression, env); },
            [env](const VariableDeclaration& node) {
                const Value val = node.init.has_value()
                                      ? evaluateExpression(*node.init.value(), env)
                                      : Value{std::monostate{}};
                env->declareVar(node.id.symbol, val);
            },
            [env](const IfStatement& node) {
                if (isTruthy(evaluateExpression(*node.test, env)))
                {
                    executeBlock(*node.thenBlock, env);
                }
                else if (node.elseBlock)
                {
                    execute(*node.elseBlock, env);
                }
            },
            [env](const WhileStatement& node) {
                while (isTruthy(evaluateExpression(*node.test, env)))
                {
                    executeBlock(*node.body, env);
                }
            },
            [env](const BlockStatement& node) { executeBlock(node, env); },
            [env](const FunctionDeclaration& node) {
                std::vector<std::string> params;
                params.reserve(node.params.size());
                for (const auto& param : node.params)
                {
                    params.push_back(param.symbol);
                }

                env->declareVar(node.id.symbol,
                                Value{FunctionValue{std::move(params), node.body.get(), env}});
            },
            [env](const ReturnStatement& node) {
                const Value value = node.argument.has_value()
                                        ? evaluateExpression(*node.argument.value(), env)
                                        : Value{std::monostate{}};
                throw ReturnSignal(value);
            }},
        stmt.node);
}

void run(const Program& program, Environment* env)
{
    for (const auto& stmt : program.body)
    {
        execute(*stmt, env);
    }
}

} // namespace scriptum
