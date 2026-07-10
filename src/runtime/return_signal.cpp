#include <scriptum/runtime/return_signal.hpp>

namespace scriptum {

ReturnSignal::ReturnSignal(Value value) : m_value(std::move(value)) {}

const Value& ReturnSignal::value() const
{
    return m_value;
}

const char* ReturnSignal::what() const noexcept
{
    return "ReturnSignal";
}

} // namespace scriptum
