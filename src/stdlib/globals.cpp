#include <scriptum/stdlib/globals.hpp>

namespace scriptum {

namespace {

void registerGlobals(Environment& env)
{
    Value printFn;
    printFn.kind = Value::Kind::Native;
    printFn.nativeFunction.arity = std::nullopt;
    printFn.nativeFunction.id = NativeFnId::Print;
    env.declareVar("print", printFn);

    Value typeFn;
    typeFn.kind = Value::Kind::Native;
    typeFn.nativeFunction.arity = 1;
    typeFn.nativeFunction.id = NativeFnId::Type;
    env.declareVar("type", typeFn);
}

} // namespace

void loadStdlib(Environment& env)
{
    registerGlobals(env);
}

} // namespace scriptum
