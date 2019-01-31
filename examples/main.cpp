#include <iostream>
#include "../include/sboc.hpp"
#include <list>
#include <map>
#include <cstddef>


uint16_t foo(std::vector<uint8_t>) { std::cout << "hihi\n"; return 1; }
auto f = [](std::vector<uint8_t>) -> uint16_t { std::cout << "hehehe"; return 1;};
int main()
{
  std::vector<uint8_t*> v;
  std::vector<int> val = {1,2,3,4};
  std::list<int> vall= {1,2,3,4};
  std::map<int, int> valm = { {1,1}, {2,2}, {3,3} };
  std::tuple<int,int,int> valt= {3,3,3};
  std::vector<std::byte> vb;
  std::string s = "aaaaaa";

  std::cout << SBOC::getSize(vall) << "\n";
  std::cout << SBOC::getSize(val)  << "\n";
  std::cout << SBOC::getSize(valt) << "\n";

  SBOC::serialize(vb, 1);


  auto ptrf = *foo;
  std::vector<uint16_t> cc;
  SBOC::test(ptrf, cc);
  return 0;
}
