#include "precompiled.hpp"

template <std::size_t N> std::size_t start_of_message(const std::string &line) {
  if (std::size(line) >= N) {
    for (std::size_t n{}; n != std::size(line) - N; ++n) {
      char range[N];
      std::copy(&line[n], &line[n + N], range);
      std::ranges::sort(range);
      if (std::empty(std::ranges::unique(range))) {
        return n + N;
      }
    }
  }
  return 0;
}

void solve(std::istream &stream) {
  std::uint64_t result1{};
  std::uint64_t result2{};

  for (std::string line; std::getline(stream, line);) {
    result1 = start_of_message<4>(line);
    result2 = start_of_message<14>(line);
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
