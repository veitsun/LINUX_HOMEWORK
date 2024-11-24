#include <iostream>
#include <thread>
using namespace std;

class Task {
public:
  void operator()(int i) { std::cout << i << std::endl; }
  // 重载()操作符，可以让Task类的对象可以像函数一样被调用。这个操作符接受一个i，并将其输出到标准输出流中
};

int main() {
  for (int i = 0; i < 4; i++) {
    Task task;
    thread t(task, i);
    t.detach();
  }
  return 0;
}