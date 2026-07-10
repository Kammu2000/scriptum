#include <stdexcept>

#include <scriptum/runtime/interpreter.hpp>
#include <scriptum/runtime/return_signal.hpp>
#include <scriptum/runtime/value.hpp>

namespace scriptum {

namespace {

Value evaluateExpression(const Expression& expr, Environment* env);

Value evaluateCall(const CallExpression& call, Environment* env)
{
    const Value fnValue = evaluateExpression(*call.callee, env);

    if (fnValue.kind == Value::Kind::Native)
    {
        if (fnValue.nativeFunction.arity.has_value() &&
            fnValue.nativeFunction.arity.value() != call.args.size())
        {
            throw std::runtime_error("Function arguments should have same length as params");
        }

        std::vector<Value> evaluatedArgs;
        evaluatedArgs.reserve(call.args.size());
        for (const auto& arg : call.args)
        {
            evaluatedArgs.push_back(evaluateExpression(*arg, env));
        }

        return callNativeFunction(fnValue.nativeFunction, evaluatedArgs, *env);
    }

    if (fnValue.kind != Value::Kind::Function)
    {
        throw std::runtime_error("No such function exists");
    }

    const FunctionValue& fn = fnValue.function;

    if (fn.params.size() != call.args.size())
    {
        throw std::runtime_error("Function arguments should have same length as params");
    }

    Environment callEnv(fn.closure);
    for (std::size_t i = 0; i < call.args.size(); ++i)
    {
        callEnv.declareVar(fn.params[i], evaluateExpression(*call.args[i], env));
    }

    try
    {
        for (const auto& stmt : fn.body->body)
        {
            execute(*stmt, &callEnv);
        }
    }
    catch (const ReturnSignal& signal)
    {
        return signal.value();
    }

    return {};
}

Value evaluateBinary(const BinaryExpression& binary, Environment* env)
{
    const Value leftValue = evaluateExpression(*binary.left, env);
    const Value rightValue = evaluateExpression(*binary.right, env);

    const double left = leftValue.kind == Value::Kind::Number ? leftValue.number : 0.0;
    const double right = rightValue.kind == Value::Kind::Number ? rightValue.number : 0.0;

    Value result;
    result.kind = Value::Kind::Number;

    if (binary.op == "+")
    {
        result.number = left + right;
    }
    else if (binary.op == "-")
    {
        result.number = left - right;
    }
    else if (binary.op == "*")
    {
        result.number = left * right;
    }
    else if (binary.op == "/")
    {
        result.number = left / right;
    }
    else if (binary.op == "%")
    {
        result.number = static_cast<int>(left) % static_cast<int>(right);
    }
    else if (binary.op == ">")
    {
        result.number = left > right ? 1.0 : 0.0;
    }
    else if (binary.op == ">=")
    {
        result.number = left >= right ? 1.0 : 0.0;
    }
    else if (binary.op == "<")
    {
        result.number = left < right ? 1.0 : 0.0;
    }
    else if (binary.op == "<=")
    {
        result.number = left <= right ? 1.0 : 0.0;
    }
    else if (binary.op == "===")
    {
        result.number = left == right ? 1.0 : 0.0;
    }
    else if (binary.op == "!==")
    {
        result.number = left != right ? 1.0 : 0.0;
    }
    else
    {
        throw std::runtime_error("Could not identify the operator " + binary.op);
    }

    return result;
}

Value evaluateExpression(const Expression& expr, Environment* env)
{
    switch (expr.kind)
    {
        case ExpressionKind::NumericLiteral: {
            Value result;
            result.kind = Value::Kind::Number;
            result.number = expr.numericLiteral.value;
            return result;
        }
        case ExpressionKind::Identifier:
            return env->lookupVar(expr.identifier.symbol);
        case ExpressionKind::AssignmentExpression: {
            const Value computedValue = evaluateExpression(*expr.assignmentExpression.right, env);
            env->assignVar(expr.assignmentExpression.left.symbol, computedValue);
            return computedValue;
        }
        case ExpressionKind::CallExpression:
            return evaluateCall(expr.callExpression, env);
        case ExpressionKind::BinaryExpression:
            return evaluateBinary(expr.binaryExpression, env);
    }

    throw std::runtime_error("Invalid expression");
}

void executeBlock(const BlockStatement& block, Environment* parentEnv)
{
    Environment childEnv(parentEnv);
    for (const auto& stmt : block.body)
    {
        execute(*stmt, &childEnv);
    }
}

} // namespace

Value evaluate(const Expression& expr, Environment* env)
{
    return evaluateExpression(expr, env);
}

void execute(const Statement& stmt, Environment* env)
{
    switch (stmt.kind)
    {
        case StatementKind::ExpressionStatement:
            evaluateExpression(*stmt.expressionStatement.expression, env);
            return;
        case StatementKind::VariableDeclaration: {
            Value val;
            if (stmt.variableDeclaration.init.has_value())
            {
                val = evaluateExpression(*stmt.variableDeclaration.init.value(), env);
            }
            env->declareVar(stmt.variableDeclaration.id.symbol, val);
            return;
        }
        case StatementKind::IfStatement: {
            if (isTruthy(evaluateExpression(*stmt.ifStatement.test, env)))
            {
                executeBlock(*stmt.ifStatement.thenBlock, env);
            }
            else if (stmt.ifStatement.elseBlock)
            {
                execute(*stmt.ifStatement.elseBlock, env);
            }
            return;
        }
        case StatementKind::WhileStatement:
            while (isTruthy(evaluateExpression(*stmt.whileStatement.test, env)))
            {
                executeBlock(*stmt.whileStatement.body, env);
            }
            return;
        case StatementKind::BlockStatement:
            executeBlock(stmt.blockStatement, env);
            return;
        case StatementKind::FunctionDeclaration: {
            Value fnValue;
            fnValue.kind = Value::Kind::Function;
            fnValue.function.params.reserve(stmt.functionDeclaration.params.size());
            for (const auto& param : stmt.functionDeclaration.params)
            {
                fnValue.function.params.push_back(param.symbol);
            }
            fnValue.function.body = stmt.functionDeclaration.body.get();
            fnValue.function.closure = env;
            env->declareVar(stmt.functionDeclaration.id.symbol, fnValue);
            return;
        }
        case StatementKind::ReturnStatement: {
            Value value;
            if (stmt.returnStatement.argument.has_value())
            {
                value = evaluateExpression(*stmt.returnStatement.argument.value(), env);
            }
            throw ReturnSignal(std::move(value));
        }
    }

    throw std::runtime_error("Statement could not be processed because of unknown syntax");
}

void run(const Program& program, Environment* env)
{
    for (const auto& stmt : program.body)
    {
        execute(*stmt, env);
    }
}

} // namespace scriptum
