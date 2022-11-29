#pragma once
#include "source_location.hpp"
#include <charconv>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <span>
#include <stdexcept>
#include <string_view>
#include <system_error>

// Wrapper for from_chars with signature like stoi etc.
// Prints error message and throws on failure.

// auto i = string_to<int>(s); // s a string, string_view or span of chars

template <std::integral Integer, typename String>
Integer string_to(String &&s, std::size_t *pos = nullptr,
  const srcloc::source_location &location =
    srcloc::source_location::current()) {
  const char *data = std::data(s);
  std::size_t size = std::size(s);
  const char *sentinel = (size == std::dynamic_extent ? nullptr : data + size);

  Integer result;
  auto [p, e] = std::from_chars(data, sentinel, result);

  if (e == std::errc::invalid_argument) {
    std::cerr << location.file_name() << ":" << location.line()
              << ": invalid argument\n";
    throw std::invalid_argument{"Invalid argument"};
  }

  if (e == std::errc::result_out_of_range) {
    std::cerr << location.file_name() << ":" << location.line()
              << ": out of range\n";
    throw std::out_of_range{"Out of range"};
  }

  if (pos) {
    *pos = p - data;
  }
  return result;
}
