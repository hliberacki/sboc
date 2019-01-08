#ifndef INCLUDE_SBOC_HPP
#define INCLUDE_SBOC_HPP
#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>
#include <numeric>
#include <tuple>
#include "experimental/concepts_ts.hpp"

namespace SBOC
{
  template<typename T>
  static size_t getSize(const T& obj);

  template<typename T, typename ...Args>
  static size_t getSize(const T& obj, const Args... args);
  
  template<class C, typename T = typename C::value_type>
  concept Container = requires(C con, T a, T b)
  {
    { con.size() }  -> size_t;
    { std::begin(con) } -> typename C::const_iterator;
    { std::end(con) } -> typename C::const_iterator;
    { a = b } -> bool;
  };

  template<class T>
  struct is_tuple : std::false_type {};

  template<class... Args>
  struct is_tuple<std::tuple<Args...>> : std::true_type {};

  template<class... Args>
  struct is_tuple<const std::tuple<Args...>> : std::true_type {};

  template<class T>
  const auto is_tuple_v = is_tuple<T>::value;

  template<typename T>
  concept Serializable = std::Integral<T>               ||
                         Container<T>                   ||
                         std::is_same_v<T, std::string> ||
                         std::is_floating_point_v<T>    ||
                         std::is_array_v<T>             ||
                         is_tuple_v<T>;

  template<typename T>
  concept SerializingContainer = Container<T> &&
                                 std::is_same_v<typename T::value_type, uint8_t*>;



  template<SerializingContainer Con, Serializable S>
  void serialize(Con con, S val)
  {
  }

  template<SerializingContainer Con, Serializable S>
  S deserialize(typename Con::const_iterator& begin, typename Con::const_iterator& end)
  {
  }

  namespace helpers::size
  {
    template<Serializable T>
    struct getSizeHelper
    {
      static size_t value(const T& obj) { return sizeof(T); }
    };

    template<Container C>
    struct getSizeHelper<C>
    {
      static size_t value(const C& obj)
      {
        return std::accumulate(obj.begin(), obj.end(), sizeof(size_t),
                               [](const size_t& sum, const typename C::value_type& current)
                               {
                                 return sum + helpers::size::getSizeHelper<typename C::value_type>::value(current);
                               });
      }
    };

    template <class Tuple, size_t... Is>
    constexpr auto getSizeTupleImpl(Tuple tuple, std::index_sequence<Is...>)
    {
       return getSize<int>(std::get<Is>(tuple) ...);
    }

    template<class ...T>
    struct getSizeHelper<std::tuple<T...>>
    {
        static size_t value(const std::tuple<T...>& obj) { return helpers::size::getSizeTupleImpl(obj, std::make_index_sequence<sizeof...(T)>{});}
    };

    template<>
    struct getSizeHelper<std::string>
    {
      static size_t value(const std::string& obj)
      {
        return sizeof(size_t) + (obj.length() * sizeof(uint8_t));
      }
    };
  }

  namespace helpers::serialize
  {

  }

  template<typename T>
  static size_t getSize(const T& obj) { return helpers::size::getSizeHelper<T>::value(obj); }

  template<typename T, typename ...Args>
  static size_t getSize(const T& obj, const Args... args) { return helpers::size::getSizeHelper<T>::value(obj) + getSize(args...); }


}
#endif //INCLUDE_SBOC_HPP
