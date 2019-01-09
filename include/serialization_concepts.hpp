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

  template<class C, typename T = typename C::value_type>
  concept Container = requires(C con, T a, T b)
  {
    { con.size() } -> size_t;
    { std::begin(con) } -> typename C::const_iterator;
    { std::end(con) } -> typename C::const_iterator;

    std::EqualityComparable<T>;
    std::Destructible<T>;
    std::CopyConstructible<T>;

    std::DefaultConstructible<C>;
    std::CopyConstructible<C>;
    std::EqualityComparable<C>;
    std::Swappable<C>;
  };

  template<typename T>
  concept Serializable = std::Integral<T>            ||
                         Container<T>                ||
                         std::Same<T, std::string>   ||
                         std::is_floating_point_v<T> ||
                         std::is_array_v<T>          ||
                         is_tuple_v<T>;

  template<typename C, typename T = typename C::value_type>
  concept SerializingContainer = Container<C> &&
                                (sizeof(std::remove_pointer_t<T>) == sizeof(uint8_t));
}

#endif //INCLUDE_SERIALIZATION_CONCEPTS_HPP
