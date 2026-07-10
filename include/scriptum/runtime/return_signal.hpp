#pragma once

#include <exception>

#include <scriptum/runtime/value.hpp>

namespace scriptum {

class ReturnSignal : public std::exception
{
  public:
    explicit ReturnSignal(Value value);

    const Value& value() const;
    const char* what() const noexcept override;

  private:
    Value m_value;
};

} // namespace scriptum
