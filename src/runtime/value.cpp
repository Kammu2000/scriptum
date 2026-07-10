#include <format>
#include <print>
#include <string>

#include <scriptum/runtime/value.hpp>

namespace scriptum {

bool isTruthy(const Value& value)
{
    switch (value.kind)
    {
        case Value::Kind::Undefined:
            return false;
        case Value::Kind::Number:
            return value.number != 0.0;
        case Value::Kind::String:
            return !value.string.empty();
        case Value::Kind::Function:
        case Value::Kind::Native:
            return true;
    }
    return false;
}

namespace {

std::string formatValue(const Value& value)
{
    switch (value.kind)
    {
        case Value::Kind::Number:
            return std::format("{}", value.number);
        case Value::Kind::String:
            return value.string;
        case Value::Kind::Undefined:
            return "undefined";
        case Value::Kind::Function:
            return "[function]";
        case Value::Kind::Native:
            return "[native function]";
    }
    return "undefined";
}

Value nativePrint(const std::vector<Value>& args, Environment&)
{
    std::string line;
    for (std::size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0)
        {
            line += ' ';
        }
        line += formatValue(args[i]);
    }
    std::println("{}", line);
    return {};
}

Value nativeType(const std::vector<Value>& args, Environment&)
{
    Value result;
    result.kind = Value::Kind::String;

    if (args.empty())
    {
        result.string = "undefined";
        return result;
    }

    switch (args[0].kind)
    {
        case Value::Kind::Undefined:
            result.string = "undefined";
            break;
        case Value::Kind::Number:
            result.string = "number";
            break;
        case Value::Kind::String:
            result.string = "string";
            break;
        case Value::Kind::Function:
            result.string = "function";
            break;
        case Value::Kind::Native:
            result.string = "function";
            break;
    }

    return result;
}

} // namespace

Value callNativeFunction(const NativeFunction& nativeFn, const std::vector<Value>& args,
                         Environment& env)
{
    switch (nativeFn.id)
    {
        case NativeFnId::Print:
            return nativePrint(args, env);
        case NativeFnId::Type:
            return nativeType(args, env);
    }
    return {};
}

} // namespace scriptum
