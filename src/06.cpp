#include "precompiled.hpp"

std::size_t start_of_message(const std::string &line, std::size_t size) {
  int histogram[26]{};
  std::size_t count_distinct{};
  if (std::size(line) >= size) {
    for (std::size_t n{}; n != size; ++n) {
      count_distinct += !histogram[line[n] - 'a']++;
    }
    for (std::size_t n{}; n != std::size(line) - size; ++n) {
      count_distinct += !histogram[line[n + size] - 'a']++;
      count_distinct -= !--histogram[line[n] - 'a'];
      if (count_distinct == size) {
        return n + size + 1;
      }
    }
  }
  return 0;
}

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  for (std::string line; std::getline(stream, line);) {
    result1 = start_of_message(line, 4);
    result2 = start_of_message(line, 14);
  }

  std::printf("Part 1 result %llu\n"
              "Part 2 result %llu\n",
    result1, result2);
}

int main() {
  constexpr int day = 6;
  std::printf("Day %d\n", day);
  for (auto pattern: {"test/%02d", "input/%02d"}) {
    auto filename = string_printf(pattern, day);
    if (std::ifstream stream(std::data(filename)); stream) {
      std::printf("\nReading file %s\n", std::data(filename));
      solve(stream);
    }
  }

  return 0;
}
