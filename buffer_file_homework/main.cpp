#include "bufferfile.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  BufferedFile file;
  // 打开文件
  if (!file.open("test.txt", "w+b")) {
    std::cerr << "failed to open file" << std::endl;
  }
  // 写入数据到文件缓存中
  const char *data = "hello world sunwei";
  const char *data2 = "yeah yeah yeah //";
  size_t written = file.write(data);
  file.write(data2);
  // 回到文件开始位置
  file.lseek(0, BufferedFile::SEEK_SET_MODE);

  // 读取数据
  char buffer[100];
  size_t read = file.read(buffer, written + 10);
  buffer[read] = '\0';
  std::cout << "Read: " << buffer << std::endl;

  file.close();

  return 0;
}