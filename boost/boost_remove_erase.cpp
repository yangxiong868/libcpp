#include <boost/range/algorithm_ext.hpp>
#include <iostream>
#include <string>

int main() 
{
  std::string s = "08:EF:13:AB:88:BI";
  boost::range::remove_erase(s, ':');
  std::cout << s << '\n';
}
