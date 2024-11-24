// cpp多线程库的简单示例

#include <cstdio>
#include <iostream>
#include <thread>

using namespace std;

void output(int i) { std::cout << i << std::endl; }

int main() {
  for (int i = 0; i < 12; i++) {
    thread t(output, i);
    t.detach();
  }

  std::cout << "所有的线程执行完毕" << std::endl;
  getchar();
  return 0;
}