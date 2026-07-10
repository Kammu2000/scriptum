#pragma once

#include <optional>
#include <string>
#include <vector>

#include <scriptum/frontend/parser/ast.hpp>

namespace scriptum {

class Environment;

struct FunctionValue
{
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
    std::optional<std::size_t> arity;
    NativeFnId id;
};

struct Value
{
    enum class Kind
    {
        Undefined,
        Number,
        String,
        Function,
        Native
    };
    Kind kind = Kind::Undefined;
    double number = 0;
    std::string string;
    FunctionValue function;
    NativeFunction nativeFunction;
};

bool isTruthy(const Value& value);
Value callNativeFunction(const NativeFunction& nativeFn, const std::vector<Value>& args,
                         Environment& env);

} // namespace scriptum
