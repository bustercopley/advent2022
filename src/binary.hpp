#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

std::string to_binary(auto value, std::size_t width) {
  std::string result;
  for (std::size_t i = 0; i != width; ++i) {
    result = static_cast<char>('0' + (value & 1)) + result;
    value >>= 1;
  }
  return result;
}

std::int64_t from_binary(const std::string &bits) {
  std::int64_t result = 0;
  for (auto c: bits) { result = (result << 1) | (c == '1'); }
  return result;
}
