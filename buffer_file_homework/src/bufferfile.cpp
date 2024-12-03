#include "bufferfile.h"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <mutex>

bool BufferedFile::flush_write_buffer() {
  if (!is_dirty || write_buffer_pos == 0) {
    return true;
  }
  size_t written = fwrite(write_buffer.data(), 1, write_buffer_pos, file);
  if (written != write_buffer_pos) {
    return false;
  }
  write_buffer_pos = 0;
  is_dirty = false;
  return true;
}

bool BufferedFile::fill_read_buffer() {
  // 从文件中读取数据到内存中
  // read_buffer.data() 指向目标缓冲区的指针
  // 每个数据块一个字节大小
  // read_buffer.size() 要读取数据块的数量
  // file 文件流
  // 将
  read_buffer_size = fread(read_buffer.data(), 1, read_buffer.size(), file);
  // std::cout << read_buffer.size() << std::endl;
  read_buffer_pos = 0; // 读缓冲区的指针归到开始位置
  return read_buffer_size > 0;
}

BufferedFile::BufferedFile() {
  file = nullptr;
  write_buffer.resize(DEFAULT_BUFFER_SIZE);
  read_buffer.resize(DEFAULT_BUFFER_SIZE);
  write_buffer_pos = 0;
  read_buffer_pos = 0;
  read_buffer_size = 0;
  file_pos = 0;
  is_dirty = false;
}

BufferedFile::~BufferedFile() { close(); }
bool BufferedFile::open(const std::string &filename, const std::string &mode) {
  std::lock_guard<std::mutex> lock(mutex);
  if (this->file) {
    close(); // 如果文件已经打开，则需要关闭文件
  }
  this->filename = filename;
  this->file = fopen(this->filename.c_str(), mode.c_str());
  if (!this->file) {
    std::cout << "打开文件失败" << std::endl;
    return false;
  }
  read_buffer_pos = 0;
  write_buffer_pos = 0;
  read_buffer_size = 0;
  file_pos = 0;
  is_dirty = false;
  return true;
}

size_t BufferedFile::write(const void *data) {
  std::lock_guard<std::mutex> lock(mutex);
  // 写操作
  if (!this->file) {
    return 0;
  }
  const char *ptr = static_cast<const char *>(data);
  size_t remaining = strlen(ptr); // 剩余待写入数据量
  size_t written = 0;             // 已经写入数据量

  while (remaining > 0) {
    // 如果写缓冲区已经满了，则需要先刷新到文件
    if (write_buffer_pos >= write_buffer.size()) {
      if (!flush_write_buffer()) {
        break; // 刷新失败
      }
    }

    // 计算本次可以写入的数据量
    size_t space = write_buffer.size() - write_buffer_pos;
    size_t to_write = std::min(space, remaining);

    // 将数据复制到写缓冲区
    memcpy(write_buffer.data() + write_buffer_pos, ptr + written, to_write);

    // 更新各种位置指针和状态
    write_buffer_pos += to_write;
    written += to_write;
    remaining -= to_write;
    file_pos += to_write;
    is_dirty = true;
  }
  return written;
}

// size_t BufferedFile::read(void *buffer) {
//   std::lock_guard<std::mutex> lock(mutex);
//   if (!file) {
//     return 0;
//   }
//   char *ptr = static_cast<char *>(buffer);
//   size_t remaining = strlen(ptr);
//   size_t read_total = 0;

//   while (remaining > 0) {
//     // 如果缓冲区已经空了，则需要重新填充
//     if (read_buffer_pos >= read_buffer_size) {
//       if (!fill_read_buffer()) {
//         break; // 填充失败 ，可能已经达到文件尾
//       }
//     }

//     // 计算本次可读取的数据量
//     size_t available = read_buffer_size - read_buffer_pos;
//     size_t to_read = std::min(available, remaining);
//     // 从读缓冲区复制数据到用户缓冲区
//     memcpy(ptr + read_total, read_buffer.data() + read_buffer_pos, to_read);
//     // 更新各种位置指针
//     read_buffer_pos += to_read;
//     read_total += to_read;
//     remaining -= to_read;
//     file_pos += to_read;
//   }
//   return read_total;
// }

size_t BufferedFile::read(void *buffer, size_t size) {
  std::lock_guard<std::mutex> lock(mutex);
  if (!file) {
    return 0;
  }
  char *ptr = static_cast<char *>(buffer);
  size_t remaining = size; // 剩余需要读出的量
  size_t read_total = 0;   // 已经读出的总量

  while (remaining > 0) {
    // std::cout << remaining << std::endl;
    // 如果缓冲区已经空了，则需要重新填充
    // 读缓冲的位置已经达到读缓冲规定的大小
    if (read_buffer_pos >= read_buffer_size) {
      // std::cout << "read_buffer_pos >= read_buffer_size" << std::endl;
      if (!fill_read_buffer()) {
        std::cout << "缓冲区不够大" << std::endl;
        break; // 填充失败 ，可能已经达到文件尾
      }
    }

    // 计算本次可读取的数据量
    size_t available = read_buffer_size - read_buffer_pos;
    size_t to_read = std::min(available, remaining);
    // 从读缓冲区复制数据到用户缓冲区
    memcpy(ptr + read_total, read_buffer.data() + read_buffer_pos, to_read);
    // 更新各种位置指针
    read_buffer_pos += to_read;
    read_total += to_read;
    remaining -= to_read;
    file_pos += to_read;
  }
  return read_total;
}

long long BufferedFile::lseek(long long offset, SeekMode mode) {
  std::lock_guard<std::mutex> lock(mutex);
  if (!file)
    return -1;
  // 如果有未写入的数据，先刷新
  if (is_dirty) {
    flush_write_buffer();
  }
  // 清空读缓冲区，因为定位操作后之前的缓存数据已经失效
  read_buffer_pos = 0;
  read_buffer_size = 0;

  // 执行文件定位
  int result = fseeko64(file, offset, static_cast<int>(mode));
  if (result == 0) {
    // 更新文件位置
    file_pos = ftello64(file);
    return file_pos;
  }
  return -1;
}

void BufferedFile::close() {
  std::lock_guard<std::mutex> lock(mutex);
  if (file) {
    flush_write_buffer();
    fclose(file);
    file = nullptr;
  }
}
