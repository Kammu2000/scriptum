#pragma once

#include <scriptum/frontend/parser/ast.hpp>
#include <scriptum/runtime/environment.hpp>

namespace scriptum {

Value evaluate(const Expression& expr, Environment* env);
void execute(const Statement& stmt, Environment* env);
void run(const Program& program, Environment* env);

} // namespace scriptum
