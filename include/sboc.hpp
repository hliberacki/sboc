#ifndef INCLUDE_SBOC_HPP
#define INCLUDE_SBOC_HPP
#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>
#include <numeric>
#include <tuple>
#include "serialization_concepts.hpp"
#include <cstddef>

namespace SBOC
{
  template<typename T>
  static size_t getSize(const T& obj);

  template<typename T, typename ...Args>
  static size_t getSize(const T& obj, const Args... args);
  

  template<SerializingContainer Con, typename S>
  void serialize(Con con, S val)
  {
		auto offset = con.size();
		auto size = helpers::size::GetSizeHelper(val);
		con.resize(offset + size);

		Con::iterator it = con.begin()+offset;
		helpers::serializer(val,it);
		assert(con.begin() + offset + size == it);
  }

  template<SerializingContainer Con, Serializable S>
  S deserialize(typename Con::const_iterator& begin, typename Con::const_iterator& end)
  {
  }

  template<Callable<uint16_t, std::vector<std::byte>> F, ComplexSerializingContainer<F, std::vector<std::byte>> C>
  void test(F f, C c)
  {
    std::vector<std::byte> v;
    std::byte myByte{ 2 };
    v.push_back(myByte);
    f(v);
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
