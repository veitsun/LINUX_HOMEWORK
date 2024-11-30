#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "SafeQueue.h"
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <pthread.h>
#include <string>
#include <thread>
#include <vector>

using namespace std;

/**
 * 线程池管理类的实现
 */
class CThreadPool {
private:
  SafeQueue<std::function<void()>> m_queue; // 任务队列，用于存储待执行的任务

  // pthread_t *pthread_id; // 线程集合，管理工作线程
  std::vector<std::thread> workers;

  // pthread_mutex_t m_pthreadMutex; // 互斥锁
  std::mutex queue_mutex;
  // pthread_cond_t m_pthreadCond;   // 条件变量
  std::condition_variable condition;

  bool shutdown; // 状态管理，控制线程池的生命周期

public:
  CThreadPool(const int threadNum);
  // 模板化的任务提交方法
  template <class F, class... Args>
  auto submit(F &&f, Args &&...args)
      -> std::future<typename std::result_of<F(Args...)>::type>;

  ~CThreadPool();
};

#endif