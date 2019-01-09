#include <iostream>
#include "../include/sboc.hpp"
#include <list>
#include <map>

int main()
{
  std::vector<uint8_t*> v;
  std::vector<int> val = {1,2,3,4};
  std::list<int> vall= {1,2,3,4};
  std::map<int, int> valm = { {1,1}, {2,2}, {3,3} };
  std::tuple<int,int,int> valt= {3,3,3};
  std::string s = "aaaaaa";

  std::cout << SBOC::getSize(vall) << "\n";
  std::cout << SBOC::getSize(val)  << "\n";
  std::cout << SBOC::getSize(valt) << "\n";

  SBOC::serialize(v, 1);
  return 0;
}
