#pragma once

#include <scriptum/frontend/parser/ast.hpp>
#include <scriptum/runtime/environment.hpp>

namespace scriptum {

void execute(const Statement& stmt, Environment* env);
void executeBlock(const BlockStatement& block, Environment* parentEnv);
void run(const Program& program, Environment* env);

} // namespace scriptum
