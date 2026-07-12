#pragma once

namespace scriptum {

/*
  Design Note:
  overloaded is a class inheriting from all types with which it will be called.
  We are initializing it with lambda functions so it will inherit from all of them.
  lambda is just a class which overloads () operator for function calling.
  so overloading is finally a class which will have many overloaded versions of () operator.
  since visit needs just one class with many overloaded versions of (), overloaded is the solution
 */
template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

/*
  Design Note:
  This is Class Template Argument Deduction (CTAD) Guide by user
  This feature came in C++17
  We need to write our own CTAD for user defined types or some complex types
  Reference article: https://devblogs.microsoft.com/cppblog/how-to-use-class-template-argument-deduction/
*/
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace scriptum
