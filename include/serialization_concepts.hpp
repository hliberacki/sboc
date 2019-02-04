#ifndef INCLUDE_SERIALIZATION_CONCEPTS_HPP
#define INCLUDE_SERIALIZATION_CONCEPTS_HPP
#include <typeinfo>
#include <string>
#include <numeric>
#include <tuple>
#include "experimental/concepts_ts.hpp"

namespace SBOC
{
  template<class T>
  struct is_tuple : std::false_type {};

  template<class... Args>
  struct is_tuple<std::tuple<Args...>> : std::true_type {};

  template<class... Args>
  struct is_tuple<const std::tuple<Args...>> : std::true_type {};

  template<class T>
  const auto is_tuple_v = is_tuple<T>::value;

  template<typename T>
  using Value_type_of = typename T::value_type;

  template<typename T>
  using Iterator_of = typename T::iterator;

  template<class C>
  concept Container = requires(C con, Value_type_of<C> a, Value_type_of<C> b)
  {
    { con.size() } -> size_t;
    { std::begin(con) } -> Iterator_of<C>;
    { std::end(con) } -> Iterator_of<C>;

    std::EqualityComparable<Value_type_of<C>>;
    std::Destructible<Value_type_of<C>>;
    std::CopyConstructible<Value_type_of<C>>;

    std::DefaultConstructible<C>;
    std::CopyConstructible<C>;
    std::EqualityComparable<C>;
    std::Swappable<C>;
  };

  template<class T>
  concept Byte = (sizeof(T) == sizeof(uint8_t));

  template<typename T>
  concept Serializable = std::Integral<T>            ||
                         Container<T>                ||
                         std::Same<T, std::string>   ||
                         std::is_floating_point_v<T> ||
                         std::is_array_v<T>          ||
                         is_tuple_v<T>;

  template<class F, class Ret, class... Args>
  concept Callable = requires(F f, Args... args)
  {
    { f(args...) } -> Ret;
  };

  template<typename C>
  concept BasicSerializingContainer = Container<C> &&
                                      Byte<std::remove_pointer_t<Value_type_of<C>>>;

  template<typename C, typename F, typename T = std::vector<uint8_t>>
  concept ComplexSerializingContainer = Container<C> &&
                                        Callable<F, Value_type_of<C>, T>;

  template<typename C, typename F = void>
  concept SerializingContainer = BasicSerializingContainer<C> ||
                                 ComplexSerializingContainer<C, F>;

}
#endif //INCLUDE_SERIALIZATION_CONCEPTS_HPP
