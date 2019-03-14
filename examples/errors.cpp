#include <iostream>
#include <string>
#include <vector>
#include "experimental/concepts_ts.hpp"

template<typename T>
using Value_type_of = typename T::value_type;

template<typename T>
using Iterator_of = typename T::iterator;

template<class R>
concept Range = requires (R range)
{
  { begin(range) } -> Iterator_of<R>;
  { end(range) } -> Iterator_of<R>;
};

template<Range R,  typename T>
 requires std::EqualityComparableWith<Value_type_of<R>, T>
 bool in(R const& range, T const& value) {
   for (auto const& x : range)
     if (x == value)
       return true;
   return false;
 }

int main()
{
  std::vector<std::string> v { "0","1", "2" };
  std::cout << std::boolalpha << in(v, 0);

  return 0;
}
