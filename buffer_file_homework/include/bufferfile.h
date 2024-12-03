#ifndef BUFFERFILE_H
#define BUFFERFILE_H

#include <cstddef>
#include <cstdio>
#include <mutex>
#include <string>
#include <vector>
class BufferedFile {
public:
  enum SeekMode {
    SEEK_SET_MODE = SEEK_SET,
    SEEK_CUR_MODE = SEEK_CUR,
    SEEK_END_MODE = SEEK_END
  };

  static constexpr size_t DEFAULT_BUFFER_SIZE = 8192;
  // 8KB的缓冲区,constexpr表示该变量是一个常量表达式，编译器在编译的时候就将其值确定。这意味着DEFAUKT_BUFFER_SIZE
  // 在程序运行时不可以更改，并且编译时使用其值进行优化

private:
  FILE *file;
  std::string filename;
  std::vector<char> write_buffer;
  std::vector<char> read_buffer;
  size_t write_buffer_pos;
  size_t read_buffer_pos;
  size_t read_buffer_size;
  size_t file_pos;
  bool is_dirty; // 写缓存是否有数据，有数据为true
  std::mutex mutex;

public:
  bool flush_write_buffer(); // 写缓冲区刷新操作
  bool fill_read_buffer();
  BufferedFile();
  ~BufferedFile();
  bool open(const std::string &filename, const std::string &mode);
  size_t write(const void *data); // 写操作
  // size_t read(void *buffer);      // 读操作
  size_t read(void *buffer, size_t size); // 读操作

  long long lseek(long long offset, SeekMode mode); // 文件定位操作
  void close();
};

#endif