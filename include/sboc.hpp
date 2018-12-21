#ifndef INCLUDE_SBOC_HPP
#define INCLUDE_SBOC_HPP

#include <string>
#include <vector>
#include <numeric>
#include <type_traits>

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
concept Serializable = Integral<T> || Container<T> || std::is_same_v<T, std::string>;

template<typename T>
concept SerializingContainer = Container<T> && std::is_same_v<typename T::value_type, uint8_t*>;

namespace sboc
{
	template<SerializingContainer Con, typename T>
	void serialize(Con con, T val)
	{
	}

	template<SerializingContainer Con, typename T>
	T deserialize(typename Con::const_iterator& begin, typename Con::const_iterator& end)
  {
	}

	namespace helpers
	{
     
	}
}

#endif //INCLUDE_SBOC_HPP
