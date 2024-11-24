// 基于pthread的多线程实现
// 给线程传入初始化参数
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#define NUM_THREADS 8
// using namespace std;

std::string messages[NUM_THREADS];

struct thread_data {
  int thread_id;
  int sum;
  const char *message;
};

thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *threadarg) {
  // long tid;
  // tid = (long)threadid;
  // std::cout << "hello world ,thread #" << tid << std::endl;
  // pthread_exit(nullptr);
  int taskid, sum;
  const char *hello_msg;
  thread_data *my_data;

  sleep(1);
  my_data = (thread_data *)threadarg;
  taskid = my_data->thread_id;
  sum = my_data->sum;
  hello_msg = my_data->message;
  std::cout << "Thread " << taskid << " " << hello_msg << " sum = " << sum
            << std::endl;
  pthread_exit(nullptr);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  // int *taskids[NUM_THREADS];
  int sum = 0;
  messages[0] = "hello world 0";
  messages[1] = "hello world 1";
  messages[2] = "hello worls 2";
  messages[3] = "hello worls 3";
  messages[4] = "ehloo worls 4";
  messages[5] = "hello world 5";
  messages[6] = "hello world 6";
  messages[7] = "hhelo world 7";

  for (long t = 0; t < NUM_THREADS; t++) {
    sum = sum + t;
    thread_data_array[t].thread_id = t;
    thread_data_array[t].message = messages[t].c_str();
    thread_data_array[t].sum = sum;

    std::cout << "In main : creating thread " << t << std::endl;

    int rc = pthread_create(&threads[t], nullptr, PrintHello,
                            (void *)&thread_data_array[t]);
    if (rc) {
      std::cout << "error; return code from pthread_create() is " << rc
                << std::endl;
      exit(-1);
    }
  }
  pthread_exit(nullptr); // main 显示调用了 pthread_exit() 来等待其他线程的结束
  // 如果不使用这个函数的话，可能main()函数结束了也有线程没有执行完毕
}