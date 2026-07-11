#pragma once

#include <scriptum/frontend/parser/ast.hpp>
#include <scriptum/runtime/environment.hpp>
#include <scriptum/runtime/value.hpp>

namespace scriptum {

Value evaluateExpression(const Expression& expr, Environment* env);

} // namespace scriptum
