#include "Solver.h"
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: sort <folder path> <destination file>\n");
    return -1;
  }

  char path[128];
  Solver solver;

  strcpy(path, argv[1]);
  if (path[strlen(path) - 1] == '/')
    path[strlen(path) - 1] = 0;

  // 得到 flag_file 文件， 遍历目录中所有文件，得到目录列表
  if (solver.get_file_list(path, "./flag_file.txt")) {
    perror("get file list error");
    return -1;
  }
  // 删除原来有的 sort_temp 目录， 新建一个空的 sort_temp 目录
  solver.delete_directory("./sort_temp");
  mkdir("./sort_temp", 0777);
  // 归并排序每单个文件
  solver.sort_list("./flag_file.txt", "./sort_temp");
  // 合并排序好的文件
  int result_prefix = solver.merge_orderd_files(0, "./sort_temp");
  // 复制结果到最终文件中
  solver.copy_prefix_file("./sort_temp", result_prefix, argv[2]);
  // solver.delete_directory("./sort_temp");
  cout << "排序完成" << endl;
  cout << "排序结果在 result.txt 文件中" << endl;
  return 0;
}