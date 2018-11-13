#include <stdint.h>
#include <iostream>
#include <string>

using namespace std;
typedef int64_t int64;
typedef int32_t int32;

struct complex {
  int32 id;
  operator std::string() const {
    std::string out;
    out.append("test id:" + std::to_string(id) + "\n");
    return out;
  }

  friend std::istream& operator>>(std::istream& in, complex& t);
  friend std::ostream& operator<<(std::ostream& out, const complex& t);
};

std::istream& operator>>(std::istream& in, complex& t) {
  in >> t.id;
  return in;
}

std::ostream& operator<<(std::ostream& out,const complex& t) {
  out << "id:" << std::to_string(t.id) << "\n";
  return out;
}

void testFunc() {
  complex test;

  std::cout << "Test operator >> overload" << std::endl;
  std::cin >> test;

  std::cout << "Test operator << overload" << std::endl;
  std::cout << test << std::endl;

  std::string str = "String ";
  str.append(test);

  std::cout << "Test operator string overload" << std::endl;
  std::cout << str << std::endl;
}

int main(int argc, char** argv) {
  testFunc();
  return 0;
}
// 运算符重载函数中用到了 complex 类的 private 成员变量，必须在 complex
// 类中将该函数声明为友元函数：
