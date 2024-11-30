#ifndef CMERGESORT_H
#define CMERGESORT_H
#include <vector>

class CMergeSort {
public:
  std::vector<int> merge(const std::vector<int> &left,
                         const std::vector<int> &right);
  std::vector<int> merge_sort(std::vector<int> &array);
};

#endif