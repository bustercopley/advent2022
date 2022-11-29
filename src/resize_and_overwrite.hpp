#pragma once

#include <string>
#include <vector>

inline void resize_and_overwrite(auto &s, auto size, auto &&op) {
#if __cpp_lib_string_resize_and_overwrite >= 202110L
  s.resize_and_overwrite(size, std::move(op));
#else
  s.resize(size); // useless value-initialization
  using std::data;
  size = op(data(s), size);
  s.resize(size);
#endif
}
