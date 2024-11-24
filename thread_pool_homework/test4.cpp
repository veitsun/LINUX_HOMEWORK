#include <iostream>
#include <thread>

using namespace std;

class _tagNode {
public:
  int a;
  int b;
};

void func(_tagNode &node) {
  node.a = 10;
  node.b = 20;
}

int main() {
  _tagNode node;
  thread t(func, std::ref(node));
  t.join();

  std::cout << node.a << std::endl;
  std::cout << node.b << std::endl;
  return 0;
}