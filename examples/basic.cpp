#include <iostream>
#include <string>
#include <vector>
#include "experimental/concepts_ts.hpp"
#include <type_traits>

template<typename T>
using Value_type_of = typename T::value_type;

template<typename T>
using Iterator_of = typename T::iterator;

template<class C>
concept Container = requires(C con)
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

template<class R>
concept Range = requires (R range)
{
  { begin(range) } -> Iterator_of<R>;
  { end(range) } -> Iterator_of<R>;
};

template<Container Con>
void sort(Con c)
{
  //something
}

template<Range Ran>
void sort(Ran r)
{
  //something else
}

int main()
{
  return 0;
}
