#include <format>
#include <stdexcept>
#include <variant>

#include <scriptum/overloaded.hpp>
#include <scriptum/runtime/evaluate_expression.hpp>
#include <scriptum/runtime/interpreter.hpp>
#include <scriptum/runtime/return_signal.hpp>
#include <scriptum/runtime/value.hpp>

namespace scriptum {

namespace {

bool valuesEqual(const Value& left, const Value& right)
{
    if (left.index() != right.index())
    {
        return false;
    }

    return std::visit(
        overloaded{[](std::monostate, std::monostate) { return true; },
                   [](double leftAlt, double rightAlt) { return leftAlt == rightAlt; },
                   [](const std::string& leftAlt, const std::string& rightAlt) {
                       return leftAlt == rightAlt;
                   },
                   [](const FunctionValue& leftAlt, const FunctionValue& rightAlt) {
                       return leftAlt.body == rightAlt.body &&
                              leftAlt.closure == rightAlt.closure &&
                              leftAlt.params == rightAlt.params;
                   },
                   [](const NativeFunction& leftAlt, const NativeFunction& rightAlt) {
                       return leftAlt.id == rightAlt.id && leftAlt.arity == rightAlt.arity;
                   },
                   [](const auto&, const auto&) -> bool {
                       throw std::runtime_error("Mismatched value types in equality check");
                   }},
        left, right);
}

double requireNumber(const Value& value, const std::string& op)
{
    if (const auto* number = std::get_if<double>(&value))
    {
        return *number;
    }
    throw std::runtime_error("Operands must be numbers for operator " + op);
}

Value evaluateCall(const CallExpression& call, Environment* env)
{
    const Value fnValue = evaluateExpression(*call.callee, env);

    if (const auto* nativeFn = std::get_if<NativeFunction>(&fnValue))
    {
        if (nativeFn->arity.has_value() && nativeFn->arity.value() != call.args.size())
        {
            throw std::runtime_error("Function arguments should have same length as params");
        }

        std::vector<Value> evaluatedArgs;
        evaluatedArgs.reserve(call.args.size());

        for (const auto& arg : call.args)
        {
            evaluatedArgs.emplace_back(evaluateExpression(*arg, env));
        }

        return callNativeFunction(*nativeFn, evaluatedArgs, *env);
    }

    const auto* fn = std::get_if<FunctionValue>(&fnValue);
    if (fn == nullptr)
    {
        throw std::runtime_error("No such function exists");
    }

    if (fn->params.size() != call.args.size())
    {
        throw std::runtime_error("Function arguments should have same length as params");
    }

    Environment callEnv(fn->closure);
    for (std::size_t i = 0; i < call.args.size(); ++i)
    {
        callEnv.declareVar(fn->params[i], evaluateExpression(*call.args[i], env));
    }

    try
    {
        for (const auto& stmt : fn->body->body)
        {
            execute(*stmt, &callEnv);
        }
    }
    catch (const ReturnSignal& signal)
    {
        return signal.value();
    }

    return Value{std::monostate{}};
}

Value evaluateBinary(const BinaryExpression& binary, Environment* env)
{
    const Value left = evaluateExpression(*binary.left, env);
    const Value right = evaluateExpression(*binary.right, env);

    if (binary.op == "+")
    {
        if (const auto* leftNum = std::get_if<double>(&left);
            leftNum != nullptr && std::holds_alternative<double>(right))
        {
            return Value{*leftNum + std::get<double>(right)};
        }

        throw std::runtime_error("Only numbers are supported for computation currently");
        // return Value{stringifyValue(left) + stringifyValue(right)};
    }
    if (binary.op == "===")
    {
        return Value{valuesEqual(left, right) ? 1.0 : 0.0};
    }
    if (binary.op == "!==")
    {
        return Value{!valuesEqual(left, right) ? 1.0 : 0.0};
    }

    const double leftNum = requireNumber(left, binary.op);
    const double rightNum = requireNumber(right, binary.op);

    if (binary.op == "-")
    {
        return Value{leftNum - rightNum};
    }
    if (binary.op == "*")
    {
        return Value{leftNum * rightNum};
    }
    if (binary.op == "/")
    {
        return Value{leftNum / rightNum};
    }
    if (binary.op == "%")
    {
        return Value{static_cast<double>(static_cast<int>(leftNum) % static_cast<int>(rightNum))};
    }
    if (binary.op == ">")
    {
        return Value{leftNum > rightNum ? 1.0 : 0.0};
    }
    if (binary.op == ">=")
    {
        return Value{leftNum >= rightNum ? 1.0 : 0.0};
    }
    if (binary.op == "<")
    {
        return Value{leftNum < rightNum ? 1.0 : 0.0};
    }
    if (binary.op == "<=")
    {
        return Value{leftNum <= rightNum ? 1.0 : 0.0};
    }

    throw std::runtime_error(std::format("Could not identify the operator: {}", binary.op));
}

} // namespace

Value evaluateExpression(const Expression& expr, Environment* env)
{
    return std::visit(
        overloaded{[](const NumericLiteral& node) { return Value{node.value}; },
                   [env](const Identifier& node) { return env->lookupVar(node.symbol); },
                   [env](const AssignmentExpression& node) {
                       const Value computedValue = evaluateExpression(*node.right, env);
                       env->assignVar(node.left.symbol, computedValue);
                       return computedValue;
                   },
                   [env](const CallExpression& node) { return evaluateCall(node, env); },
                   [env](const BinaryExpression& node) { return evaluateBinary(node, env); }},
        expr.node);
}

} // namespace scriptum
