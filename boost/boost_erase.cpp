#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

using namespace boost::algorithm;

int main() 
{
  std::string s = "08:EF:13:AB:88:BI";
  s.erase(std::remove(s.begin(), s.end(), ':'), s.end());
  std::cout << s << '\n';
}
