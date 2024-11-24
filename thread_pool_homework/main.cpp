#include "include/Thread.h"
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class CMyTask : public CTask {
public:
  CMyTask() {}

  inline int Run() {
    printf("%s\n", (char *)this->m_ptrData);
    sleep(10);
    return 0;
  }
};

int main() {
  CMyTask taskObj;

  char szTmp[] = "this is the new thread running";
  taskObj.SetData((void *)szTmp);
  CThreadPool threadPool(10);

  for (int i = 0; i < 20; i++) {
    threadPool.AddTask(&taskObj);
  }

  while (1) {
    printf("there are still %d tasks need to handle\n",
           threadPool.getTaskSize());
    if (threadPool.getTaskSize() == 0) {
      if (threadPool.StopAll() == -1) {
        printf("Now I will exit from main\n");
        exit(0);
      }
    }
    sleep(2);
  }

  return 0;
}