#include <scriptum/stdlib/globals.hpp>

namespace scriptum {

namespace {

void registerGlobals(Environment& env)
{
    env.declareVar("print", Value{NativeFunction{NativeFnId::Print, std::nullopt}});
    env.declareVar("type", Value{NativeFunction{NativeFnId::Type, 1}});
}

} // namespace

void loadStdlib(Environment& env)
{
    registerGlobals(env);
}

} // namespace scriptum
