#include "string_printf.hpp"
#include "resize_and_overwrite.hpp"
#include <cstdarg>

std::string string_printf(const char *fmt, ...) {
  std::string result;
  std::va_list args1;
  va_start(args1, fmt);
  std::va_list args2;
  va_copy(args2, args1);
  auto len = std::vsnprintf(nullptr, 0, fmt, args1);
  va_end(args1);
  resize_and_overwrite(result, len + 1, [&](auto data, auto size) {
    std::vsnprintf(data, size, fmt, args2);
    return len;
  });
  va_end(args2);
  return result;
}
