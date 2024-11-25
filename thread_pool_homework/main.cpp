#include "include/Thread.h"
#include <algorithm>
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

// 归并排序，合并两个已经排序的数组
vector<int> merge(const vector<int> &left, const vector<int> &right) {
  vector<int> result;
  auto it1 = left.begin();
  auto it2 = right.begin();
  while (it1 != left.end() && it2 != right.end()) {
    if (*it1 < *it2) {
      result.push_back(*it1);
      ++it1;
    } else {
      result.push_back(*it2);
      ++it2;
    }
  }
  result.insert(result.end(), it1, left.end());
  result.insert(result.end(), it2, right.end());

  return result;
}

// 归并排序的步骤
vector<int> merge_sort(vector<int> &array) {
  if (array.size() <= 1) { // 递归结束的条件判断
    return array;
  }
  // 否则的话，对两边进行归并排序
  size_t mid = array.size() / 2;
  vector<int> left(array.begin(), array.begin() + mid);
  vector<int> right(array.begin() + mid, array.end());

  // 递归的对两边进行归并排序
  auto left_ret = merge_sort(left);
  auto right_ret = merge_sort(right);

  return merge(left_ret, right_ret);
  // merge_sort(left);
  // merge_sort(right);
  // return merge(left, right);
}

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