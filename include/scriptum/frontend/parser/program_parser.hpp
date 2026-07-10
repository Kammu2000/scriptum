#pragma once

#include <string>

#include <scriptum/frontend/parser/ast.hpp>

namespace scriptum {

class ProgramParser
{
  public:
    Program buildAST(const std::string& sourceCode);
};

} // namespace scriptum
