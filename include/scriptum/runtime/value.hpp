#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <scriptum/frontend/parser/ast.hpp>

namespace scriptum {

class Environment;

struct FunctionValue
{
    FunctionValue(std::vector<std::string> params, const BlockStatement* body,
                  Environment* closure);

    std::vector<std::string> params;
    const BlockStatement* body = nullptr;
    Environment* closure = nullptr;
};

enum class NativeFnId
{
    Print,
    Type
};

struct NativeFunction
{
    // Design Note: arity is number of params a native function accepts
    // std::nullopt means function can accept any number of params
    NativeFunction(NativeFnId id, std::optional<std::size_t> arity = std::nullopt);

    std::optional<std::size_t> arity;
    NativeFnId id;
};

using Value = std::variant<std::monostate, double, std::string, FunctionValue, NativeFunction>;

bool isTruthy(const Value& value);
std::string stringifyValue(const Value& value);
Value callNativeFunction(const NativeFunction& nativeFn, const std::vector<Value>& args,
                         Environment& env);

} // namespace scriptum
