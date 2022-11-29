#pragma once

// Provide srcloc::source_location (from standard library if available)

#if __has_include(<source_location>)
#include <source_location>
namespace srcloc = std;
#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>
namespace srcloc = std::experimental;
#else
namespace srcloc {
struct source_location {
  source_location(const char *file_name, int line):
      m_file_name(file_name), m_line(line) {}
  static source_location current(const char *file_name = __builtin_FILE(),
    int line = __builtin_LINE()) {
    return source_location{file_name, line};
  }

  const char *file_name() const { return m_file_name; }
  int line() const { return m_line; }

private:
  const char *m_file_name;
  int m_line;
};
}
#endif
