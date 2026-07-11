#include <format>
#include <print>
#include <string>

#include <scriptum/overloaded.hpp>
#include <scriptum/runtime/value.hpp>

namespace scriptum {

FunctionValue::FunctionValue(std::vector<std::string> params, const BlockStatement* body,
                               Environment* closure)
    : params(std::move(params)), body(body), closure(closure)
{
}

NativeFunction::NativeFunction(NativeFnId id, std::optional<std::size_t> arity)
    : arity(arity), id(id)
{
}

bool isTruthy(const Value& value)
{
    return std::visit(
        overloaded{
            [](std::monostate) { return false; },
            [](double alternative) { return alternative != 0.0; },
            [](const std::string& alternative) { return !alternative.empty(); },
            [](const auto&) { return true; }},
        value);
}

std::string stringifyValue(const Value& value)
{
    return std::visit(
        overloaded{
            [](double alternative) -> std::string { return std::format("{}", alternative); },
            [](const std::string& alternative) -> std::string { return alternative; },
            [](std::monostate) -> std::string { return "undefined"; },
            [](const FunctionValue&) -> std::string { return "[function]"; },
            [](const NativeFunction&) -> std::string { return "[native function]"; }},
        value);
}

namespace {

Value nativePrint(const std::vector<Value>& args, Environment&)
{
    std::string line;
    for (std::size_t i = 0; i < args.size(); ++i)
    {
        if (i > 0)
        {
            line += ' ';
        }
        line += stringifyValue(args[i]);
    }
    std::println("{}", line);
    return Value{std::monostate{}};
}

Value nativeType(const std::vector<Value>& args, Environment&)
{
    if (args.empty())
    {
        return Value{std::string{"undefined"}};
    }

    return std::visit(
        overloaded{
            [](std::monostate) { return Value{std::string{"undefined"}}; },
            [](double) { return Value{std::string{"number"}}; },
            [](const std::string&) { return Value{std::string{"string"}}; },
            [](const auto&) { return Value{std::string{"function"}}; }},
        args[0]);
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
    return Value{std::monostate{}};
}

} // namespace scriptum
