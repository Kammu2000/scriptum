#include <format>
#include <stdexcept>

#include <scriptum/runtime/environment.hpp>

namespace scriptum {

Environment::Environment(Environment* parent) : m_parent(parent) {}

void Environment::declareVar(const std::string& name, const Value& value)
{
    if (m_variables.contains(name))
    {
        throw std::runtime_error(std::format("Cannot redefine variable: {}", name));
    }
    m_variables[name] = value;
}

void Environment::assignVar(const std::string& name, const Value& value)
{
    Environment* env = resolveEnv(name);
    env->m_variables.at(name) = value;
}

Value Environment::lookupVar(const std::string& name) const
{
    const Environment* env = resolveEnv(name);
    return env->m_variables.at(name);
}

Environment* Environment::resolveEnv(const std::string& name) const
{
    if (m_variables.contains(name))
    {
        return const_cast<Environment*>(this);
    }

    if (m_parent == nullptr)
    {
        throw std::runtime_error(std::format("{}: is not defined anywhere in program", name));
    }

    return m_parent->resolveEnv(name);
}

} // namespace scriptum
