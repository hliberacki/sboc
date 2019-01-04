#ifndef INCLUDE_SBOC_HPP
#define INCLUDE_SBOC_HPP
#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>
#include <numeric>

namespace SBOC
{
  template<typename T>
  static size_t getSize(const T& obj);

  template<typename T, typename ...Args>
  static size_t getSize(const T& obj, const Args... args);
  
	template<typename T>
  concept Container = requires(T con)
  {
    { con.size() };
    { std::begin(con) };
    { std::end(con) };
  };

  template<class T>
  concept Integral = std::is_integral_v<T>;

  template<typename T>
  concept Serializable = Integral<T>                    ||
                         Container<T>                   ||
                         std::is_same_v<T, std::string> ||
                         std::is_floating_point_v<T>    ||
                         std::is_array_v<T>;

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

  namespace helpers
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
                                 return sum + helpers::getSizeHelper<typename C::value_type>::value(current);
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
        static size_t value(const std::tuple<T...>& obj) { return helpers::getSizeTupleImpl(obj, std::make_index_sequence<sizeof...(T)>{});}
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

  template<typename T>
  static size_t getSize(const T& obj) { return helpers::getSizeHelper<T>::value(obj); }

  template<typename T, typename ...Args>
  static size_t getSize(const T& obj, const Args... args) { return helpers::getSizeHelper<T>::value(obj) + getSize(args...); }


}
#endif //INCLUDE_SBOC_HPP
