#pragma once

#include <string>
#include <unordered_map>

#include <scriptum/runtime/value.hpp>

namespace scriptum {

class Environment
{
  public:
    explicit Environment(Environment* parent);

    void declareVar(const std::string& name, const Value& value);
    void assignVar(const std::string& name, const Value& value);
    Value lookupVar(const std::string& name) const;

  private:
    Environment* resolveEnv(const std::string& name) const;

    std::unordered_map<std::string, Value> m_variables;
    Environment* m_parent;
};

} // namespace scriptum
